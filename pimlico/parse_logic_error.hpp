#pragma once

#include <sstream>
#include <string>

namespace Pimlico {

class ParseLogicError : public SyntaxError {

public:

    ParseLogicError(const std::string &message, const TextBuffer &buffer) :
            SyntaxError(message, buffer) {}

};

};
