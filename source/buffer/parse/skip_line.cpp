#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

void Parse::skip_line() {
    if(this->position.line >= indextations.size()) {
        this->position.index = this->length;
        this->position.column = this->length - indextations.back().first;
    }
    else {
        this->position.index = indextations[this->position.line].first;
        this->position.line += 1;
        this->position.column = 1;
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
