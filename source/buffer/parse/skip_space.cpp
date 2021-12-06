#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

void Parse::skip_space() {
    while(true) {
        if(this->position.index == this->length)
            return;

        char character = this->text[this->position.index];
        if(character == ' '
                || character == '\t'
                || character == '\r'
                || character == '\v'
                || character == '\n')
            this->increment();
        else
            break;
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
