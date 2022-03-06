#include "excerpt.hpp"

namespace Pimlico {

Excerpt::Excerpt(const std::string &source,
        const std::string &message,
        const ParseBuffer &buffer) {

    this->source = source;
    this->message = message;
    this->text = buffer.line_text();
    this->position = buffer.position;
}

Excerpt::Excerpt(const std::string &source,
        const std::string &message,
        const ParseBuffer &buffer,
        const ParseBuffer::ParseBuffer::Position &position) {

    this->source = source;
    this->message = message;
    this->text = buffer.line_text(position.line);
    this->position = position;
}

}; // Namespace Pimlico
