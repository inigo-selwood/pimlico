#pragma once

#include <string>

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Reference : public Term {

public:

    std::string name;

    Term *value;

    Reference() : Term(Term::Type::REFERENCE) {}

    static Reference *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
