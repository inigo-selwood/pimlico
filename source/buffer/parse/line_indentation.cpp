#include <iostream>

#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

char Parse::line_indentation(long line_number) const {
    if(line_number <= 0)
        line_number = this->position.line;
    if(line_number < 1 || line_number > this->indextations.size())
        return 0;

    return this->indextations[line_number - 1].second;
}

}; // Namespace Buffer

}; // Namespace Pimlico
