#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "error/error.hpp"

#include "../parse_buffer/parse_buffer.hpp"
#include "../text_position/text_position.hpp"

class ErrorBuffer {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const ErrorBuffer &buffer);

    void add(const std::string &message);
    void add(const std::string &message, const ParseBuffer &buffer);
    void add(const std::string &message,
            const ParseBuffer &buffer,
            const TextPosition &position);

    void print(std::ostream &stream) const;

    ~ErrorBuffer();

private:

    std::vector<Error *> errors;

};
