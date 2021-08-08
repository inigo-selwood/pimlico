#pragma once

#include <vector>

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

class Choice : public Term {

public:

    std::vector<Term *> values;

    Choice() : Term(Term::Type::CHOICE) {}

    static Term *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
