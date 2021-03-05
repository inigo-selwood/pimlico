#pragma once

#include <functional>
#include <ostream>
#include <regex>
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
                const Position &position) {
            return stream << '['
                    << position.line << ':'
                    << position.column << ']';
        }

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

    // Used during whitespace skipping
    std::function<bool(TextBuffer &)> comment_skip_function;

    Position position;

    // Constructor
    TextBuffer(const std::string &text);

    // Status
    bool end_reached() const;
    int indentation_delta(unsigned long reference);

    // Meta information
    std::string line_text(unsigned long number) const;
    unsigned int line_indentation(unsigned long number) const;

    // Increment
    void increment(const unsigned long steps);

    // Non-consuming read
    char peek() const;
    bool peek(const std::string &string) const;
    bool peek(const char &character) const;
    std::string peek(const std::regex &regex) const;

    // Consuming read
    char read();
    bool read(const std::string &string);
    bool read(const char &character);
    std::string read(const std::regex &regex);

    // Skip
    void skip_line(const bool overflow);
    void skip_space(const bool overflow);
    void skip_whitespace();

private:

    std::string text;

    std::vector<unsigned int> line_indentations;
    std::vector<unsigned long> line_indices;

    unsigned long line_count;
    unsigned long length;

    // Internal read
    inline char get_character() const;
    inline std::string get_string(const long &string_length) const;

};

// ***************************************************************** Constructor

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

    if(text[length - 1] == '\n') {
        line_indentations.push_back(0);
        line_indices.push_back(length - 1);
    }

    line_count = line_indentations.size();

    position.block_indentation = line_indentations.front();
    const unsigned int indentation_target = position.block_indentation + 8;
    position.line_broken = position.line < line_count
            && line_indentations[position.line] >= indentation_target;
}

// ********************************************************************** Status

// Checks whether the buffer has reached the end of its text
bool TextBuffer::end_reached() const {
    return (position.index + 1) >= length;
}

/* Finds the difference in indentation between the current line and the next

The next line is considered the next non-empty line

Arguments:
    reference: if present, the difference in indentation is calculated between
        it and the next
*/
int TextBuffer::indentation_delta(unsigned long reference = 0) {
    if(reference == 0)
        reference = position.line;

    if(reference >= line_count)
        return 0;

    const Position start_position = position;
    skip_whitespace();
    const unsigned int next_line = position.line;
    position = start_position;

    return line_indentations[next_line - 1] - line_indentations[reference - 1];
}

// ************************************************************ Meta information

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

    unsigned int end_index = start_index;
    while(true) {
        if(end_index == length || text[end_index] == '\n')
            break;
        end_index += 1;
    }

    const unsigned int length = end_index - start_index;
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

    return line_indentations[number - 1];
}

// *********************************************************** Position changing

/* Increments the buffer's position

Arguments:
    steps: the number of steps to increment the buffer's position by
*/
void TextBuffer::increment(const unsigned long steps = 1) {
    for(unsigned int index = 0; index < steps; index += 1) {
        if(position.index >= length)
            return;
        position.index += 1;

        if(text[position.index - 1] == '\n') {

            const unsigned int indentation_target =
                    position.block_indentation + 8;
            if(position.line < line_count
                    && line_indentations[position.line] >= indentation_target)
                position.line_broken = true;
            else {
                position.line_broken = false;

                if(position.line < line_count) {
                    position.block_indentation =
                            line_indentations[position.line];
                }
            }

            position.line += 1;
            position.column = 1;
        }
        else if(text[position.index] == '\t') {
            const unsigned int remainder = position.column % 5;
            position.column += (5 - remainder);
        }
        else if(text[position.index] != '\r')
            position.column += 1;
    }
}

// ********************************************************** Non-consuming read

// Returns the current character
char TextBuffer::peek() const {
    return get_character();
}

// Returns true if the given string is found
bool TextBuffer::peek(const std::string &string) const {
    return get_string(string.length()) == string;
}

// Returns true if the character given is found
bool TextBuffer::peek(const char &character) const {
    return get_character() == character;
}

std::string TextBuffer::peek(const std::regex &regex) const {
    static constexpr std::regex_constants::match_flag_type flags =
        std::regex_constants::match_continuous
            | std::regex_constants::match_not_null;

    const auto start = text.cbegin() + (position.index - 1);
    const auto end = text.cbegin() + line_indices[position.line];
    std::smatch matches;
    const bool result_count = std::regex_match(start,
            end,
            matches,
            regex,
            flags);

    if(result_count == 0)
        return "";

    const std::string &first_match = matches[0];
    return first_match;
}

// ************************************************************** Consuming read

// Consumes and returns the current character
char TextBuffer::read() {
    const char &character = get_character();
    increment();
    return character;
}

// Returns true if the given string is found, consuming it
bool TextBuffer::read(const std::string &string) {
    if(get_string(string.length()) != string)
        return false;

    increment(string.length());
    return true;
}

// Returns true if the given character is found, consuming it
bool TextBuffer::read(const char &character) {
    if(get_character() != character)
        return false;

    increment();
    return true;
}

std::string TextBuffer::read(const std::regex &regex) {
    const std::string result = peek(regex);
    if(result.empty() == false)
        increment(result.length());
    return result;
}

// ************************************************************************ Skip

/* Skips characters until the next newline (or the buffer's end)

Arguments:
    overflow: if true, skips past broken newlines too
*/
void TextBuffer::skip_line(const bool overflow = false) {
    while(true) {
        skip_space(overflow);
        if(end_reached() || text[position.index - 1] == '\n')
            return;

        increment();
    }
}

/* Skips whitespace characters

Whitespace includes: comments, spaces, tabs, newlines, and line feeds
*/
void TextBuffer::skip_whitespace() {
    while(true) {
        const char character = get_character();
        if(character == ' ' || character == '\t' || character == '\r' || character == '\n')
            increment();
        else if(comment_skip_function != nullptr
                && comment_skip_function(*this))
            continue;
        else
            break;
    }
}

/* Skips "soft" whitespace characters

"Soft" whitespace includes: comments, spaces, tabs, and line feeds

Arguments:
    overflow: if true, skips past broken newlines too
*/
void TextBuffer::skip_space(const bool overflow = false) {
    while(true) {
        if(end_reached())
            return;

        // Handle spaces, tabs, line-feeds and newlines
        else if(text[position.index] == ' '
                || text[position.index] == '\t'
                || text[position.index] == '\r'
                || (text[position.index] == '\n'
                    && position.line_broken
                    && overflow))
            increment();

        // Handle comments
        else if(comment_skip_function != nullptr
                && comment_skip_function(*this))
            continue;

        else
            break;
    }
}

// *************************************************************** Internal read

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
