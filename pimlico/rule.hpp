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

    std::shared_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

std::shared_ptr<Rule> Rule::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    const auto rule = std::shared_ptr<Rule>(new Rule());

    // Parse name
    std::string name;
    while(true) {
        if(buffer.end_reached())
            break;

        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            name += buffer.read();
        else
            break;
    }

    if(name.empty())
        throw;
    rule->name.push_back(name);

    // Check semi-colon present between name and value
    buffer.skip_space();
    if(buffer.read(':') == false) {
        const SyntaxError error("expected ':' after rule name", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check rule has value
    buffer.skip_space();
    if(buffer.read('\n')) {
        if(buffer.line_indentation(buffer.line + 1) ==
                buffer.line_indentation(buffer.line) + 2)
            buffer.skip_whitespace();
        else {
            const SyntaxError error("rule must contain terms", buffer);
            errors.push_back(error);
            return nullptr;
        }
    }

    // Parse rule value
    rule->value = Term::parse(buffer, errors);
    if(rule->value == nullptr)
        return nullptr;

    return rule;
}

};
