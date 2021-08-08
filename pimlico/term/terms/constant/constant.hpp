#pragma once

#include <string>

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Constant : public Term {

public:

    std::string value;

    Constant() : Term(Term::Type::CONSTANT) {}

    static Constant *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
