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
        column_number = buffer.position.column_number;
        line_number = buffer.position.line_number;
    }

    friend std::ostream &operator<<(std::ostream &stream, const SyntaxError &error) {
        stream << "[" << error.line_number << ":" << error.column_number << "] "
                << error.message << "\n"
                << error.text << "\n";

        for(unsigned int index = 1; index < error.column_number; index += 1)
            stream << '.';
        stream << '^';

        return stream;
    }

};

};
