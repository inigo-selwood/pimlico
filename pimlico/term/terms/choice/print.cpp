#include <ostream>

#include "choice.hpp"

void Choice::print(std::ostream &stream) const {
    bool enclosed = false;
    if(this->bounds != std::array<int, 2>({1, 1})) {
        enclosed = true;
        stream << '(';
    }

    for(int index = 0; index < this->values.size(); index += 1) {
        Term *term = this->values[index];

        bool child_enclosed = false;
        if(term->type == Term::Type::SEQUENCE) {
            stream << '(';
            child_enclosed = true;
        }

        stream << *term;
        if(child_enclosed)
            stream << ')';

        if(index + 1 < values.size())
            stream << " | ";
    }

    if(enclosed)
        stream << ')';
}