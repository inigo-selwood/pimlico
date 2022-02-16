#include "message.hpp"

namespace Pimlico {

namespace Buffer {

void Message::print(std::ostream &stream) const {
    stream << "(" << this->source << ") " << this->text;
}

}; // Namespace Buffer

}; // Namespace Pimlico
