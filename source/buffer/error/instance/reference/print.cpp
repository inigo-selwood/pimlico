#include "reference.hpp"

namespace Pimlico {

namespace Buffer {

void Reference::print(std::ostream &stream) const {
    stream << this->position << ' ' << this->message << '\n';
    stream << "    " << this->text << '\n';
}

}; // Namespace Buffer

}; // Namespace Pimlico
