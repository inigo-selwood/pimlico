#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

// Skip space (not including newlines)
void Parse::skip_space() {
    while(true) {

        // Stop iterating if the end of the buffer has been reached
        if(this->position.index == this->length)
            return;

        // Skip spaces, tabs, and carriage returns
        char character = this->text[this->position.index];
        if(character == '#')
            this->skip_line();
        else if(character == ' '
                || character == '\t'
                || character == '\r'
                || character == '\v')
            this->increment();
        else
            break;
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
