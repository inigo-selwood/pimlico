#include "parse_buffer.hpp"

// Skips everything, newlines included
void ParseBuffer::skip_whitespace() {
    while(true) {
        if(this->position.index == this->length)
            return;

        char character = this->text[this->position.index];
        if(character != ' ' &&
                character != '\t' &&
                character != '\n' &&
                character != '\r' &&
                character != '\v')
            break;

        this->position.index += 1;
    }
}

void ParseBuffer::skip_space(const bool &overflow) {
    while(true) {
        if(this->position.index == this->length)
            return;

        char character = this->text[this->position.index];
        if(character == ' '
                || character == '\t'
                || character == '\r'
                || character == '\v'
                || (character == '\n'
                    && position.line_broken
                    && overflow))
            increment();

        else
            break;
    }
}

void ParseBuffer::skip_line(const bool &overflow) {
    TextPosition &position = this->position;
    while(true) {
        if(position.index >= this->length)
            return;

        bool ignore_newline = (position.line_broken && overflow);
        if(this->text[position.index] == '\n' && ignore_newline == false)
            return

        increment();
    }
}
