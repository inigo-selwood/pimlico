#include <ostream>
#include <array>

#include "sequence.hpp"

void Sequence::print(std::ostream &stream) const {
    bool enclosed = false;
    if(this->bounds != std::array<int, 2>({1, 1})) {
        enclosed = true;
        stream << '(';
    }

    for(int index = 0; index < this->values.size(); index += 1) {
        Term *term = this->values[index];

        stream << *term;

        if(index + 1 < values.size())
            stream << ' ';
    }

    if(enclosed)
        stream << ')';
}
