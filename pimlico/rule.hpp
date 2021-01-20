#pragma once

#include <memory>
#include <string>
#include <vector>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "term.hpp"

namespace Pimlico {

class Rule {

public:

    std::vector<std::string> name;

    std::shared_ptr<Term> value;

    unsigned int hash;

    bool inlined;

    bool hangar;

    std::unique_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

};
