#pragma once

#include "text_buffer.hpp"

namespace Pimlico {

struct SyntaxError {

public:

    std::string message;
    std::string text;

    unsigned int column;
    unsigned long line;

    SyntaxError(const std::string &message, const TextBuffer &buffer) {
        this->message = message;

        text = buffer.line_text();
        column = buffer.column;
        line = buffer.line;
    }

};

};
