#include "parse_buffer.hpp"

namespace Pimlico {

// Skips whitespace (including newlines)
void ParseBuffer::skip_whitespace() {
    while(true) {

        // Stop iterating if the end of the buffer has been reached
        if(this->position.index == this->length)
            return;

        // Continue if the character is non-whitespace
        char character = this->text[this->position.index];
        if(character == '#')
            this->skip_line();
        else if(character == ' '
                || character == '\t'
                || character == '\r'
                || character == '\v'
                || character == '\n')
            this->increment();
        else
            break;
    }
}

}; // Namespace Pimlico
