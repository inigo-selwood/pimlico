#pragma once

#include <memory>
#include <vector>
#include <string>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "rule.hpp"

namespace Pimlico {

class Specification {

public:

    Specification(const std::string &grammar);

    std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<SyntaxError> &errors);

private:

    std::vector<std::shared_ptr<Rule>> rules;

};

};
