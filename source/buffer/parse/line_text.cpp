#include <iostream>

#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

std::string Parse::line_text(long line_number) const {
    if(line_number <= 0)
        line_number = this->position.line;
    else if(line_number < 1 || line_number > this->indextations.size())
        return std::string();

    long start_index = 0;
    if(line_number > 1)
        start_index = this->indextations[line_number - 1].first;

    long end_index = this->length;
    if(line_number < this->indextations.size())
        end_index = this->indextations[line_number].first - 1;
    else if(this->length && this->text[this->length - 1] == '\n')
        end_index = this->length - 1;

    long length = end_index - start_index;
    return this->text.substr(start_index, length);
}

}; // Namespace Buffer

}; // Namespace Pimlico
