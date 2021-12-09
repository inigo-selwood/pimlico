#include "message.hpp"

namespace Pimlico {

namespace Buffer {

std::ostream &operator<<(std::ostream &stream, const Message &message) {
    message.print(stream);
    return stream;
}

void Message::print(std::ostream &stream) const {
    stream << this->text;
}

}; // Namespace Buffer

}; // Namespace Pimlico
