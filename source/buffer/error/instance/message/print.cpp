#include "message.hpp"

namespace Pimlico {

namespace Buffer {

void Message::print(std::ostream &stream) const {
    stream << this->text << '\n';
}

}; // Namespace Buffer

}; // Namespace Pimlico
