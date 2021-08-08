#pragma once

#include <vector>

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Sequence : public Term {

public:

    std::vector<Term *> values;

    Sequence() : Term(Term::Type::SEQUENCE) {}

    static Term *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
