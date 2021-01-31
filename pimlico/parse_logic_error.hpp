#pragma once

#include <sstream>
#include <string>

#include "text_buffer.hpp"

namespace Pimlico {

class ParseLogicError : public TextBuffer::SyntaxError {

public:

    ParseLogicError(const std::string &message, const TextBuffer &buffer) :
            SyntaxError(message, buffer) {}

};

};
