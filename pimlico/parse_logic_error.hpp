#pragma once

#include <exception>
#include <sstream>
#include <string>

namespace Pimlico {

class ParseLogicError : public SyntaxError {

public:

    ParseLogicError(const std::string &message, const TextBuffer &buffer) :
            SyntaxError(message, buffer) {}

    const char *what() const throw() {
        std::stringstream stream;
        stream << *this;
        return stream.str().c_str();
    }

};

};
