#include "reference.hpp"

namespace Pimlico {

namespace Buffer {

void Reference::print(std::ostream &stream) const {
    if(this->text.empty() == false) {
        stream << this->position
                << " (" << this->source << ") "
                << this->message << '\n';
        stream << "    " << this->text;
    }
    else
        stream << "(" << this->source << ") " << this->message;
}

}; // Namespace Buffer

}; // Namespace Pimlico
