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

    std::string name;

    std::vector<std::string> scope;

    std::variant<std::shared_ptr<Term>,
            std::vector<std::shared_ptr<Rule>>> value;

    bool terminal;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule);

    static std::shared_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

std::ostream &operator<<(std::ostream &stream, const Rule &rule) {

    if(rule.terminal) {
        const std::shared_ptr<Term> value =
                std::get<std::shared_ptr<Term>>(rule.value);

        if(rule.scope.empty() == false) {
            for(unsigned int index = 0;
                    index < rule.scope.size() + 1;
                    index += 1)
                stream << "    ";
        }
        stream << rule.name << ": " << *value;
    }
    else {
        const std::vector<std::shared_ptr<Rule>> children =
                std::get<std::vector<std::shared_ptr<Rule>>>(rule.value);

        for(unsigned int index = 0; index < rule.scope.size(); index += 1)
            stream << "    ";
        stream << rule.name << "...\n";

        for(const auto &child : children)
            stream << *child << "\n";
    }

    return stream;
}

std::shared_ptr<Rule> Rule::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());

    buffer.skip_space();
    while(true) {
        if(buffer.end_reached())
            break;

        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            rule->name += buffer.read();
        else
            break;
    }

    buffer.skip_space();
    if(rule->name.empty())
        throw ParseLogicError("expected rule name", buffer);

    // Handle rules
    if(buffer.read(':')) {
        rule->terminal = true;

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
    else if(buffer.read("...")) {
        const TextBuffer::Position start_position = buffer.position;

        rule->terminal = false;

        buffer.skip_space();
        bool errors_found = false;
        if(buffer.peek('\n') == false) {
            SyntaxError error("trailing characters after name-extended rule",
                    buffer);
            errors.push_back(error);
            errors_found = true;
        }

        const unsigned int start_indentation =
                buffer.line_indentations[buffer.position.line_number - 1];

        std::vector<std::shared_ptr<Rule>> children;
        while(true) {

            // Check if the next line is a single-indented child
            buffer.skip_whitespace();
            const unsigned int new_indentation =
                    buffer.line_indentations[buffer.position.line_number - 1];
            if(buffer.position.line_number >= buffer.line_indentations.size()
                    || new_indentation <= start_indentation)
                break;

            // Parse the child
            const auto child = Rule::parse(buffer, errors);

            if(child == nullptr)
                errors_found = true;
            child->scope.push_back(rule->name);
            children.push_back(child);

            if(buffer.end_reached() == false && buffer.peek('\n') == false)
                throw ParseLogicError("incomplete rule parse", buffer);
        }

        if(errors_found == true)
            return nullptr;
        else if(children.empty()) {
            buffer.position = start_position;
            const std::string message = "no children found for name-extended "
                    "rule " + rule->name;
            SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        rule->value = children;
        return rule;
    }

    return rule;
}

};
