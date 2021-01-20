#pragma once

#include <memory>
#include <string>
#include <vector>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "specification.hpp"
#include "token.hpp"

namespace Pimlico {

class SyntaxTree {

public:

    Token root;

    std::unique_ptr<SyntaxTree> parse(const Specification &specification,
            const std::string &source,
            const std::vector<SyntaxError> &errors);

};

};
