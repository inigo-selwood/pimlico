#pragma once

#include "text_buffer.hpp"

namespace Pimlico {

struct SyntaxError {

public:

    std::string message;
    std::string text;

    unsigned int column_number;
    unsigned long line_number;

    SyntaxError(const std::string &message, const TextBuffer &buffer) {
        this->message = message;

        text = buffer.line_text();
        column_number = buffer.column_number;
        line_number = buffer.line_number;
    }

};

};
