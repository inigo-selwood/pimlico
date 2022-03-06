#include "parse_buffer.hpp"

namespace Pimlico {

// Skips the current line
void ParseBuffer::skip_line() {

    // If the next line is the end of the text, skip to the end of the buffer
    if(this->position.line >= indextations.size()) {
        this->position.index = this->length;
        this->position.column = this->length - indextations.back().first;
    }

    // Otherwise, skip to the start index of the next line
    else {
        this->position.index = indextations[this->position.line].first;
        this->position.line += 1;
        this->position.column = 1;
    }
}

}; // Namespace Pimlico
