#include "reference.hpp"

namespace Pimlico {

namespace Buffer {

Reference::Reference(const std::string &message, const Parse &buffer) {
    this->message = message;
    this->text = buffer.line_text();
    this->position = buffer.position;
}

Reference::Reference(const std::string &message,
        const Parse &buffer,
        const Position &position) {

    this->message = message;
    this->text = buffer.line_text(position.line);
    this->position = position;
}

}; // Namespace Buffer

}; // Namespace Pimlico
