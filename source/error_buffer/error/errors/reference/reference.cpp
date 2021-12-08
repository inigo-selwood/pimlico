#include "reference.hpp"

Reference::Reference(const std::string &message, const ParseBuffer &buffer) {
    this->message = message;
    this->position = buffer.position;
    this->text = buffer.line_text(true);
}

Reference::Reference(const std::string &message,
        const ParseBuffer &buffer,
        const TextPosition &position) {

    this->message = message;
    this->position = position;
    this->text = buffer.line_text(position.line);
}
