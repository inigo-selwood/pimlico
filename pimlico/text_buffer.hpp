#pragma once

#include <functional>
#include <ostream>
#include <string>
#include <vector>

namespace Pimlico {

class TextBuffer {

public:

    struct Position {

    public:

        unsigned long index;

        unsigned long line;
        unsigned int column;

        friend std::ostream &operator<<(std::ostream &stream,
                const Position &position);

        Position() : index(0),
                line(1),
                column(1),
                block_indentation(0),
                line_broken(false) {}

    private:

        friend class TextBuffer;

        unsigned int block_indentation;

        bool line_broken;

    };

    struct SyntaxError {

    public:

        struct Reference {

            TextBuffer::Position position;

            std::string text;
            std::string message;

            friend std::ostream &operator<<(std::ostream &stream,
                    const Reference &reference);

            Reference(const std::string &message,
                    const TextBuffer &buffer,
                    const TextBuffer::Position *position);

        };

        std::vector<Reference> references;

        friend std::ostream &operator<<(std::ostream &stream,
            const SyntaxError &error);

        SyntaxError() {}
        SyntaxError(const std::string &message,
                const TextBuffer &buffer,
                const TextBuffer::Position *position);

        void add_reference(const std::string &message,
                const TextBuffer &buffer,
                const TextBuffer::Position *position);

    };

    std::function<bool(TextBuffer &)> comment_skip_function;

    Position position;

    TextBuffer(const std::string &text);

    bool valid(std::vector<SyntaxError> &errors);

    bool end_reached() const;
    int indentation_delta(unsigned long reference) const;

    std::string line_text(unsigned long number) const;
    unsigned int line_indentation(unsigned long number) const;

    void increment(const unsigned long steps);

    char peek() const;
    bool peek(const std::string &string) const;
    bool peek(const char &character) const;

    char read();
    bool read(const std::string &string);
    bool read(const char &character);

    void skip_line(const bool overflow);
    void skip_space(const bool overflow);
    void skip_whitespace();

private:

    std::string text;

    std::vector<unsigned int> line_indentations;
    std::vector<unsigned long> line_indices;

    unsigned long line_count;
    unsigned long length;

    inline char get_character() const;
    inline std::string get_string(const long &string_length) const;

};

std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::Position &position) {
    return stream << '[' << position.line << ':' << position.column << ']';
}

std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::SyntaxError::Reference &reference) {

    stream << reference.position << ' ' << reference.message << '\n'
            << reference.text << '\n';

    for(unsigned int index = 1; index < reference.position.index; index += 1)
        stream << ' ';
    stream << '^';
}

TextBuffer::SyntaxError::Reference::Reference(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position) {

    this->position = (position) ? *position : buffer.position;
    this->message = message;
    this->text = buffer.line_text(this->position.line);
}

std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::SyntaxError &error) {

    const unsigned int reference_count = error.references.size();
    for(unsigned int index = 0; index < reference_count; index += 1) {
        stream << error.references[index];
        if(index + 1 < reference_count)
            stream << '\n';
    }
}

TextBuffer::SyntaxError::SyntaxError(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position = nullptr) {
    references.push_back(Reference(message, buffer, position));
}

void TextBuffer::SyntaxError::add_reference(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position) {
    references.push_back(Reference(message, buffer, position));
}

// Constructor
TextBuffer::TextBuffer(const std::string &text) {
    this->text = text;

    length = text.length();

    for(unsigned int offset = 0; offset < length; offset += 1) {
        line_indices.push_back(offset);

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

        while(offset < length && text[offset] != '\n')
            offset += 1;

        line_indentations.push_back(line_indentation);
    }

    if(text[length - 1] == '\n')
        line_indentations.push_back(0);

    line_count = line_indentations.size();

    position.block_indentation = line_indentations.front();
    const unsigned int indentation_target = position.block_indentation + 8;
    position.line_broken = position.line < line_count
            && line_indentations[position.line] >= indentation_target;
}

/* Checks whether the buffer's text is valid

- Checks indentation levels are multiples of 4
- Checks characters are valid (< 0x20 or > 0x7E)
*/
bool TextBuffer::valid(std::vector<TextBuffer::SyntaxError> &errors) {

    bool valid = true;

    // Check indentation levels
    for(unsigned int index = 0; index < line_indentations.size(); index += 1) {
        const unsigned int &indentation = line_indentations[index];
        if(indentation % 4) {
            Position position;
            position.line = index + 1;
            position.index = line_indices[index];

            SyntaxError error("invalid indentation", *this, &position);
            errors.push_back(error);
            valid = false;
        }
    }

    // Check character validity
    const Position start_position = position;
    while(end_reached() == false) {
        skip_whitespace();
        const char character = read();
        if(character < ' ' || character > '~') {
            SyntaxError error("invalid character", *this);
            errors.push_back(error);
            valid = false;
        }
    }
    position = start_position;

    return valid;
}

// Checks whether the buffer has reached the end of its text
bool TextBuffer::end_reached() const {
    return position.index >= length;
}

/* Finds the difference in indentation between the current line and the next

The next line is considered the next non-empty line

Arguments:
    reference: if present, the difference in indentation is calculated between
        it and the next
*/
int TextBuffer::indentation_delta(unsigned long reference = 0) const {
    if(reference == 0)
        reference = position.line;

    if(reference >= line_count)
        return 0;

    const int reference_indentation = line_indentations[reference];

    unsigned int next_line = position.line;
    for(unsigned long index = position.index; index < length; index += 1) {

        // Increment the line count
        if(text[position.index - 1] == '\n' && index > position.index)
            next_line += 1;

        if(index == length)
            return 0;

        // Skip whitespace
        else if(text[position.index] == ' '
                || text[position.index] == '\t'
                || text[position.index] == '\r')
            continue;

        // Skip comments
        else if(text[position.index] == '#') {
            while(true) {
                if(index == length)
                    return 0;
                else if(text[position.index - 1] == '\n')
                    break;

                index += 1;
            }
        }

        // Stop when a non-whitespace character has been found
        else
            break;
    }

    if(next_line == line_count)
        return 0;

    const unsigned int &next_indentation = line_indentations[next_line];

    return reference_indentation - next_indentation;
}

/* Gets the current line's text

Arguments:
    number: if present, the text of the given line is returned
*/
std::string TextBuffer::line_text(unsigned long number = 0) const {

    // Assign and check the line number
    if(number == 0)
        number = position.line;
    else if(number > line_count)
        return "";

    const unsigned int start_index = line_indices[number - 1];

    // Calculate the substring's length
    unsigned long length = 0;
    if(number == line_count)
        length = std::string::npos;
    else
        length = line_indices[number] - start_index;

    return text.substr(start_index, length);
}

/* Gets the current line's text

Arguments:
    number: if present, the indentation of a given line is returned
*/
unsigned int TextBuffer::line_indentation(unsigned long number = 0) const {
    if(number == 0)
        number = position.line;
    else if(number > line_count)
        return 0;
    else
        return line_indentations[number];
}

/* Increments the buffer's position

Arguments:
    steps: the number of steps to increment the buffer's position by
*/
void TextBuffer::increment(const unsigned long steps = 1) {
    for(unsigned int index = 0; index < steps; index += 1) {
        if(position.index + 1 >= length)
            return;
        position.index += 1;

        if(text[position.index - 1] == '\n') {
            position.line += 1;
            position.column = 1;

            const unsigned int indentation_target =
                    position.block_indentation + 8;
            if(position.line < line_count
                    && line_indentations[position.line] >= indentation_target)
                position.line_broken = true;
            else {
                position.line_broken = false;

                if(position.line + 1 < line_count) {
                    position.block_indentation =
                            line_indentations[position.line + 1];
                }
            }
        }
        else if(text[position.index] == '\t') {
            const unsigned int remainder = position.column % 5;
            position.column += (5 - remainder);
        }
        else if(text[position.index] != '\r')
            position.column += 1;
    }
}

// Returns the current character
char TextBuffer::peek() const {
    return get_character();
}

// Returns true if the given string is found
bool TextBuffer::peek(const std::string &string) const {
    return text.substr(position.index, string.length()) == string;
}

// Returns true if the character given is found
bool TextBuffer::peek(const char &character) const {
    return text[position.index] == character;
}

// Consumes and returns the current character
char TextBuffer::read() {
    const char &character = text[position.index];
    increment();
    return character;
}

// Returns true if the given string is found, consuming it
bool TextBuffer::read(const std::string &string) {
    if(text.substr(position.index, string.length()) != string)
        return false;

    increment(string.length());
    return true;
}

// Returns true if the given character is found, consuming it
bool TextBuffer::read(const char &character) {
    if(text[position.index] != character)
        return false;

    increment();
    return true;
}

/* Skips characters until the next newline (or the buffer's end)

Arguments:
    overflow: if true, skips past broken newlines too
*/
void TextBuffer::skip_line(const bool overflow = false) {
    while(true) {
        skip_space(overflow);
        if(end_reached() || text[position.index - 1] == '\n')
            return

        increment();
    }
}

/* Skips whitespace characters

Whitespace includes: comments, spaces, tabs, newlines, and line feeds
*/
void TextBuffer::skip_whitespace() {
    while(true) {
        while(position.index < length && text[position.index] == '\n')
            increment();

        const unsigned long start_index = position.index;
        skip_space(true);
        if(position.index == length || position.index == start_index)
            return;
    }
}

/* Skips "soft" whitespace characters

"Soft" whitespace includes: comments, spaces, tabs, and line feeds

Arguments:
    overflow: if true, skips past broken newlines too
*/
void TextBuffer::skip_space(const bool overflow = false) {
    while(true) {

        if(position.index == length)
            return;

        // Handle spaces, tabs, line-feeds and newlines
        else if(text[position.index] == ' '
                || text[position.index] == '\t'
                || text[position.index] == '\r'
                || (text[position.index] == '\n' && position.line_broken))
            increment();

        // Handle comments
        else if(comment_skip_function != nullptr
                && comment_skip_function(*this))
            continue;

        else
            break;
    }
}

// Gets the current character
inline char TextBuffer::get_character() const {
    return (position.index < length) ? text[position.index] : 0;
}

// Gets a string of a given length at the current index
inline std::string TextBuffer::get_string(const long &string_length) const {
    if(position.index + string_length > length)
        return "";

    return text.substr(position.index, string_length);
}

};
