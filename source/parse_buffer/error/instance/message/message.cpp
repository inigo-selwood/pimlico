#include "message.hpp"

namespace Pimlico {

Message::Message(const std::string &source, const std::string &text) {
    this->source = source;
    this->text = text;
}

}; // Namespace Pimlico
