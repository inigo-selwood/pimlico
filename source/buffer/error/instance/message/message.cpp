#include "message.hpp"

namespace Pimlico {

namespace Buffer {

Message::Message(const std::string &source, const std::string &text) {
    this->source = source;
    this->text = text;
}

}; // Namespace Buffer

}; // Namespace Pimlico
