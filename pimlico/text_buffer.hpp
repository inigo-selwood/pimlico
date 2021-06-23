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
    int indentation_delta(unsigned long line_number);

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

/* Prints a text buffer position

Prints a position in the format "[line:column]"

Arguments:
    stream (std::ostream &): the stream to print to
    position (const TextBuffer::Position &): the position struct to print

Returns:
    stream (std::ostream &): the stream that's been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::Position &position) {
    return stream << '[' << position.line << ':' << position.column << ']';
}

/* Prints a syntax error reference

Arguments:
    stream (std::ostream &): the stream to print to
    reference (const TextBuffer::SyntaxError::Reference &): the syntax error
        reference struct to print

Returns:
    stream (std::ostream &): the stream that's been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::SyntaxError::Reference &reference) {

    stream << reference.position << ' ' << reference.message << '\n'
            << reference.text << '\n';

    for(unsigned int index = 1; index < reference.position.column; index += 1)
        stream << '.';
    stream << '^';
    return stream;
}

// Constructor
TextBuffer::SyntaxError::Reference::Reference(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position = nullptr) {

    this->position = position ? *position : buffer.position;
    this->message = message;
    this->text = buffer.line_text(this->position.line);
}

/* Prints a syntax error

Arguments:
    stream (std::ostream &): the stream to print to
    error (const TextBuffer::SyntaxError &): the syntax error to print

Returns:
    stream (std::ostream &): the stream that's been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const TextBuffer::SyntaxError &error) {

    const unsigned int reference_count = error.references.size();
    for(unsigned int index = 0; index < reference_count; index += 1) {
        stream << error.references[index];
        if(index + 1 < reference_count)
            stream << '\n';
    }
    return stream;
}

// Constructor
TextBuffer::SyntaxError::SyntaxError(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position = nullptr) {
    references.push_back(Reference(message, buffer, position));
}

/* Add a reference to a given syntax error

Arguments:
    message (const std::string &): the message to display with the new
        entry
    buffer (const TextBuffer &): the buffer in which the reference is
        located
    position (const TextBuffer::Position *): a pointer to the position in the
        buffer where the syntax error was encountered, or nullptr to use the
        buffer's current position
*/
void TextBuffer::SyntaxError::add_reference(const std::string &message,
        const TextBuffer &buffer,
        const TextBuffer::Position *position = nullptr) {
    references.push_back(Reference(message, buffer, position));
}

// Constructor
TextBuffer::TextBuffer(const std::string &text) {
    this->text = text;

    length = text.length();

    // Record the indentation on each line
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

    // Add an empty un-indented line if the text ends in a newline
    if(text[length - 1] == '\n') {
        line_indentations.push_back(0);
        line_indices.push_back(length - 1);
    }
    line_count = line_indentations.size();

    // Initialize the buffer's position's indentation value
    position.block_indentation = line_indentations.front();
    const unsigned int indentation_target = position.block_indentation + 8;
    position.line_broken = position.line < line_count
            && line_indentations[position.line] >= indentation_target;
}

/* Check whether the buffer's text is valid

- Checks indentation levels are multiples of 4
- Checks characters are valid (< 0x20 or > 0x7E)

To do:
    - Check for unicode characters

Arguments:
    errors (std::vector<TextBuffer::SyntaxError> &): a vector to store recorded
        errors in

Returns:
    valid (bool): true if the buffer contains text in a valid format
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
        if(character && (character < ' ' || character > '~')) {
            SyntaxError error("invalid character", *this);
            errors.push_back(error);
            valid = false;
        }
    }
    position = start_position;

    return valid;
}

/* Checks whether the buffer has reached the end of its text

Returns:
    end_reached (bool): true if the end of the buffer has been reached
*/
bool TextBuffer::end_reached() const {
    return (position.index + 1) >= length;
}

/* Find the difference in indentation between the current line and the next

The next line is considered the next non-empty line

Arguments:
    line_number (unsigned long): if given, the difference in indentation is
        calculated against the line of the same number

Returns:
    difference: the difference in indentation between the current line and
        the next line (or given) line
*/
int TextBuffer::indentation_delta(unsigned long line_number = 0) {
    if(line_number == 0)
        line_number = position.line;

    if(line_number >= line_count)
        return 0;

    const Position start_position = position;
    skip_whitespace();
    const unsigned int next_line = position.line;
    position = start_position;

    return line_indentations[next_line - 1] -
            line_indentations[line_number - 1];
}

/* Gets the current line's text

Arguments:
    number: if present, the text of the given line is returned

Returns:
    text (std::string): the text on the given line
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

Returns:
    indentation (unsigned int): the indentation level on the line specified
*/
unsigned int TextBuffer::line_indentation(unsigned long number = 0) const {
    if(number == 0)
        number = position.line;
    else if(number > line_count)
        return 0;

    return line_indentations[number - 1];
}

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

/* Returns the current character without incrementing the buffer's index

Returns:
    character (char): the character at the buffer's current index, or 0 if the
        end of the buffer has been reached
*/
char TextBuffer::peek() const {
    return get_character();
}

/* Checks for the presence of a given string at the buffer's position

Arguments:
    string (const std::string &): the string to check for

Returns:
    found (bool): true if the given string was found at the buffer's current
        position
*/
bool TextBuffer::peek(const std::string &string) const {
    return get_string(string.length()) == string;
}

/* Checks for the presence of a given character at the buffer's current position

Arguments:
    character (const char &): the character to check for

Returns:
    found (bool): true if the given character was found at the buffer's
        current position
*/
bool TextBuffer::peek(const char &character) const {
    return get_character() == character;
}

/* Consumes and returns the current character

Returns:
    character (char): the character at the buffer's position at the time of this
        function being called -- now the previous position. Or 0, if the end
        of the buffer has been reached
*/
char TextBuffer::read() {
    const char &character = get_character();
    increment();
    return character;
}

/* Check for (and if found, consume) a given string

Arguments:
    string (const std::string &): the string to check for and consume

Returns:
    found (bool): true if the given string was found at the buffer's current
        position
*/
bool TextBuffer::read(const std::string &string) {
    if(get_string(string.length()) != string)
        return false;

    increment(string.length());
    return true;
}

/* Checks for (and if found, consume) a given character

Arguments:
    character (const char &): the character to check for

Returns:
    found (bool): true if the given character was found at the buffer's
        current position
*/
bool TextBuffer::read(const char &character) {
    if(get_character() != character)
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
