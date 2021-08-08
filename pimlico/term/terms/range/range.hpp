#pragma once

#include <array>

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Range : public Term {

public:

    std::array<char, 2> values;

    Range() : Term(Term::Type::RANGE) {}

    static Range *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
