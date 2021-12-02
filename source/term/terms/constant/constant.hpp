#pragma once

#include <string>

#include "../parse_escape_code.hpp"

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Constant : public Term {

public:

    std::string value;

    Constant();

    static Constant *parse(ParseBuffer &buffer, ErrorBuffer &errors);

    void print(std::ostream &stream) const override;

};
