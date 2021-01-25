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

    std::vector<std::shared_ptr<Rule>> children;

    std::shared_ptr<Rule> parent;

    std::shared_ptr<Term> value;

    std::string name;

    bool inlined;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule);

    static std::shared_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

std::ostream &operator<<(std::ostream &stream, const Rule &rule) {
    return stream << rule.name << ": " << *rule.value;
}

std::shared_ptr<Rule> Rule::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

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

    buffer.skip_space();
    if(name.empty())
        throw ParseLogicError("expected rule name", buffer);

    std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());
    rule->name = name;
    rule->inlined = name[0] == '_';

    // Handle rules
    if(buffer.read(':')) {

        // Check there are terms present
        buffer.skip_space();
        if(buffer.end_reached() || buffer.peek('\n')) {
            SyntaxError error("expected terms", buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Parse the rule's value
        const auto value = Term::parse(buffer, errors, true);
        if(value == nullptr)
            return nullptr;
        else if(buffer.peek('\n') == false)
            throw ParseLogicError("incomplete term parse", buffer);

        rule->value = value;
        return rule;
    }

    // Handle name extensions
    else if(buffer.read("..."))
        throw;

    return rule;
}

};
