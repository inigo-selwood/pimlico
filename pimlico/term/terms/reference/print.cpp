#include "reference.hpp"

void Reference::print(std::ostream &stream) const {
    stream << this->name;
}
