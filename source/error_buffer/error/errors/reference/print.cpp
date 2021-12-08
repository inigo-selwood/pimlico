#include <ostream>

#include "reference.hpp"

std::ostream &operator<<(std::ostream &stream, const Reference &reference) {
    reference.print(stream);
    return stream;
}

void Reference::print(std::ostream &stream) const {
    stream << this->position << ' ' << this->message << '\n';
    stream << this->text;
}
