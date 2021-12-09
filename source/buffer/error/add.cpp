#include "error.hpp"

#include "instance/message/message.hpp"
#include "instance/reference/reference.hpp"

namespace Pimlico {

namespace Buffer {

void Error::add(const std::string &text) {
    this->instances.push_back(new Message(text));
}

void Error::add(const std::string &text, const Parse &buffer) {
    this->instances.push_back(new Reference(text, buffer));
}

void Error::add(const std::string &text,
        const Parse &buffer,
        const Position &position) {
    
    this->instances.push_back(new Reference(text, buffer, position));
}

}; // Namespace Buffer

}; // Namespace Pimlico
