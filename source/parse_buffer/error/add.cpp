#include "../parse_buffer.hpp"

#include "instance/message/message.hpp"
#include "instance/excerpt/excerpt.hpp"

namespace Pimlico {

void ParseBuffer::Error::add(const std::string &source,
        const std::string &text) {

    this->instances.push_back(new Message(source, text));
}

void ParseBuffer::Error::add(const std::string &source,
        const std::string &text,
        const ParseBuffer &buffer) {

    this->instances.push_back(new Excerpt(source, text, buffer));
}

void ParseBuffer::Error::add(const std::string &source,
        const std::string &text,
        const ParseBuffer &buffer,
        const ParseBuffer::Position &position) {

    this->instances.push_back(new Excerpt(source, text, buffer, position));
}

}; // Namespace Pimlico
