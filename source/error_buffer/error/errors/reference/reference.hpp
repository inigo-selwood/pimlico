#pragma once

#include <ostream>
#include <string>

#include "../../error.hpp"

#include "../../../../parse_buffer/parse_buffer.hpp"
#include "../../../../text_position/text_position.hpp"

class Reference : public Error {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Reference &error);

    Reference(const std::string &message, const ParseBuffer &buffer);
    Reference(const std::string &message,
            const ParseBuffer &buffer,
            const TextPosition &position);

    void print(std::ostream &stream) const override;

private:

    TextPosition position;

    std::string message;
    std::string text;

};
