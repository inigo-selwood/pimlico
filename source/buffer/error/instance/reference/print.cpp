#include "reference.hpp"

namespace Pimlico {

namespace Buffer {

void Reference::print(std::ostream &stream) const {
    if(this->text.empty() == false) {
        stream << this->position << ' ' << this->message << '\n';
        stream << "    " << this->text;
    } else
        stream << this->message;
}

}; // Namespace Buffer

}; // Namespace Pimlico
