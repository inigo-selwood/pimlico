#include "message.hpp"

namespace Pimlico {

void Message::print(std::ostream &stream) const {
    stream << "(" << this->source << ") " << this->text;
}

}; // Namespace Pimlico
