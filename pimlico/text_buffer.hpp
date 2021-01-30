#pragma once

#include <functional>
#include <string>
#include <vector>

namespace Pimlico {

class TextBuffer {

public:

    struct Position {

    public:

        unsigned long index;

        unsigned long line_number;
        unsigned long column_number;

        friend std::ostream &operator<<(std::ostream &stream,
                const Position &position) {

            return stream << "(line " << position.line_number <<
                    ", column " << position.column_number << ")";
        }

    private:

        friend class TextBuffer;

        unsigned int line_start_indentation;

        bool line_extended;

    };

    std::function<bool(TextBuffer &)> comment_skip_function;

    std::vector<unsigned int> line_indentations;

    Position position;

    TextBuffer(const std::string &text);

    bool end_reached() const;

    void increment(const unsigned int steps);

    std::string line_text() const;

    int indentation_delta(long reference_line);
    unsigned int indentation(long line_number) const;

    char peek() const;
    bool peek(const std::string &string) const;
    bool peek(const char &character) const;

    char read();
    bool read(const std::string &string);
    bool read(const char &character);

    void skip_line(const bool overflow_line);
    void skip_space(const bool overflow_line);
    void skip_whitespace();
    void skip_block();

private:

    std::string text;

    unsigned long line_count;

    unsigned long length;

    inline char get_character() const;
    inline std::string get_string(const long &string_length);

};

TextBuffer::TextBuffer(const std::string &text) {
    this->text = text;
    position.index = 0;
    position.line_number = 1;
    position.column_number = 1;

    length = text.length();

    for(unsigned int offset = 0; offset < text.size(); offset += 1) {
        unsigned int line_indentation = 0;
        while(true) {
            if(offset == text.size())
                break;
            else if(text[offset] == '\t')
                line_indentation += 4;
            else if(text[offset] == ' ')
                line_indentation += 1;
            else
                break;

            offset += 1;
        }

        while(offset < text.size() && text[offset] != '\n')
            offset += 1;

        line_indentations.push_back(line_indentation);
    }

    if(text[length - 1] == '\n')
        line_indentations.push_back(0);

    line_count = line_indentations.size();

    position.line_start_indentation = line_indentations.front();
    const unsigned int indentation_target = position.line_start_indentation + 8;
    position.line_extended = position.line_number < line_count
            && line_indentations[position.line_number] >= indentation_target;
}

bool TextBuffer::end_reached() const {
    return position.index >= length;
}

void TextBuffer::increment(const unsigned int steps = 1) {
    for(long step = 0; step < steps; step += 1) {
        if(position.index == length)
            return;

        position.index += 1;
        if(text[position.index - 1] != '\r')
            position.column_number += 1;

        if(text[position.index - 1] == '\n') {
            position.line_number += 1;
            position.column_number = 1;

            const unsigned int indentation_target = position.line_start_indentation + 8;
            if(position.line_number < line_count
                    && line_indentations[position.line_number] >= indentation_target)
                position.line_extended = true;
            else {
                position.line_extended = false;

                if(position.line_number + 1 < line_count) {
                    position.line_start_indentation =
                            line_indentations[position.line_number + 1];
                }
            }
        }
    }
}

std::string TextBuffer::line_text() const {
    unsigned long start_index = position.index;
    while(true) {
        if(start_index == 0 || text[start_index - 1] == '\n')
            break;
        start_index -= 1;
    }

    unsigned long end_index = position.index;
    while(true) {
        if(end_index == length || text[end_index] == '\n')
            break;
        end_index += 1;
    }

    const unsigned long length = end_index - start_index;
    return text.substr(start_index, length);
}

int TextBuffer::indentation_delta(long reference_line = -1) {

    // Check the reference line
    if(reference_line == -1) {
        reference_line = position.line_number;

        if(reference_line == line_count)
            return 0;
    }
    else if(reference_line < 1 || reference_line > line_count)
        return 0;

    const Position start_position = position;

    // Skip to the end of the current line
    while(end_reached() == false && get_character() != '\n')
        increment();

    // Skip to the next non-empty line, store the number, and reset the position
    skip_whitespace();
    const unsigned long target_line = position.line_number;
    position = start_position;

    return line_indentations[target_line - 1] -
            line_indentations[reference_line - 1];
}

unsigned int TextBuffer::indentation(long line_number = -1) const {
    if(line_number == -1)
        return line_indentations[position.line_number - 1];

    if(line_number < 1 || line_number > line_count)
        return 0;
    else
        return line_indentations[line_number];
}

char TextBuffer::peek() const {
    return get_character();
}

bool TextBuffer::peek(const char &character) const {
    return get_character() == character;
}

bool TextBuffer::peek(const std::string &string) const {
    return text.substr(position.index, string.length()) == string;
}

char TextBuffer::read() {
    const char result = get_character();
    increment();
    return result;
}

bool TextBuffer::read(const char &character) {
    if(get_character() == character) {
        increment();
        return true;
    }
    return false;
}

bool TextBuffer::read(const std::string &string) {
    const long length = string.length();
    bool result = text.substr(position.index, length) == string;
    if(result)
        increment(length);
    return result;
}

void TextBuffer::skip_space(const bool overflow_line = false) {
    long start_index = position.index;
    while(position.index < length) {

        // Handle spaces and tabs
        const auto character = get_character();
        if(character == ' ') {
            position.index += 1;
            position.column_number += 1;
        }
        else if(character == '\r')
            position.index += 1;
        else if(character == '\t') {
            position.index += 1;

            const int remainder = position.column_number % 5;
            position.column_number += (5 - remainder);
        }
        else if(overflow_line && character == '\n' && position.line_extended)
            increment();

        // Handle comments
        else if(comment_skip_function != nullptr &&
                comment_skip_function(*this))
            continue;

        // Failing the above, break out
        else
            break;
    }
}

void TextBuffer::skip_whitespace() {
    long start_index = position.index;
    while(true) {
        skip_space();
        if(read('\n') == false)
            break;
    }
}

void TextBuffer::skip_line(const bool overflow_line = false) {
    const long start_index = position.index;
    while(true) {
        if(position.index < length && text[position.index - 1] == '\n')
            position.index += 1;

        while(position.index < length && text[position.index - 1] != '\n')
            position.index += 1;

        if(overflow_line
                && position.index < length
                && text[position.index] == '\n')
            increment();
        else
            break;
    }

    if(position.index >= length)
        return;
    position.column_number = 1;
    position.line_number += 1;
}

void TextBuffer::skip_block() {
    while(true) {
        skip_space(true);
        if(end_reached())
            break;
        else if(peek('\n')) {
            if(position.line_extended)
                increment();
            else
                break;
        }
        else
            increment();
    }
}

inline char TextBuffer::get_character() const {
    return (position.index < length) ? text[position.index] : 0;
}

inline std::string TextBuffer::get_string(const long &string_length) {
    return (position.index + string_length < length) ?
            text.substr(position.index, string_length) : "";
}

};
