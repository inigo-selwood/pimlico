#pragma once

#include <memory>
#include <string>
#include <vector>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "term.hpp"

namespace Pimlico {

class Rule {

private:

    void add_parent_scope(const std::string &parent);

public:

    std::variant<std::shared_ptr<Term>,
            std::vector<std::shared_ptr<Rule>>> value;

    std::vector<std::string> scope;

    std::string name;

    bool terminal;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule);

    static std::shared_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

// Recursively adds a parent scope to this rule and its children
void Rule::add_parent_scope(const std::string &parent) {
    scope.push_back(parent);
    if(terminal == false) {
        const std::vector<std::shared_ptr<Rule>> children =
                std::get<std::vector<std::shared_ptr<Rule>>>(value);
        for(const auto &child : children)
            child->add_parent_scope(parent);
    }
}

std::ostream &operator<<(std::ostream &stream, const Rule &rule) {

    // Indent the rule
    for(unsigned int index = 0; index < rule.scope.size(); index += 1)
        stream << "    ";

    // Serialize terminal rules
    if(rule.terminal) {
        const std::shared_ptr<Term> value =
                std::get<std::shared_ptr<Term>>(rule.value);
        stream << rule.name << ": " << *value;
    }

    // Serialize non-terminal rules (ie: rules with children)
    else {
        const std::vector<std::shared_ptr<Rule>> children =
                std::get<std::vector<std::shared_ptr<Rule>>>(rule.value);

        stream << rule.name << "...\n";

        for(const auto &child : children) {
            stream << *child;
            if(child->terminal)
                stream << '\n';
        }
    }

    return stream;
}

// Parse a rule
std::shared_ptr<Rule> Rule::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    // Create a rule instance
    std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());

    // Extract the rule's 'snake_case' name
    buffer.skip_space();
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

    // Check there was a name at the index given
    if(name.empty())
        throw ParseLogicError("expected rule name", buffer);

    // Handle rules
    buffer.skip_space();
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
    }

    // Handle "non-terminal" name extended rules
    else if(buffer.read("...")) {
        rule->terminal = false;

        // Hold the start position for error reporting
        const TextBuffer::Position start_position = buffer.position;

        // Check the line is empty after the ellipsis
        buffer.skip_space();
        if(buffer.peek('\n') == false) {
            SyntaxError error("trailing characters after name-extended rule",
                    buffer);
            errors.push_back(error);
            rule = nullptr;
        }

        // Keep track of the start indentation
        const unsigned int start_indentation =
                buffer.line_indentations[buffer.position.line_number - 1];

        // Parse children (rules defined below this one, indented by 4 spaces)
        std::vector<std::shared_ptr<Rule>> children;
        while(true) {

            // Check if the next line is a single-indented child
            const unsigned int new_indentation =
                    buffer.line_indentations[buffer.position.line_number];
            if(buffer.position.line_number >= buffer.line_indentations.size()
                    || new_indentation <= start_indentation)
                break;
            else
                buffer.skip_whitespace();

            // Parse the child
            const auto child = Rule::parse(buffer, errors);
            if(child == nullptr)
                rule = nullptr;
            else if(buffer.end_reached() == false && buffer.peek('\n') == false)
                throw ParseLogicError("incomplete rule parse", buffer);

            // Scope the child appropriately and add it to the vector
            child->add_parent_scope(name);
            children.push_back(child);
        }

        if(children.empty()) {
            buffer.position = start_position;
            const std::string message = "no children found for name-extended "
                    "rule " + name;
            SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        if(rule)
            rule->value = children;
    }

    // Check there was either a semi-colon or an ellipsis found
    else {
        SyntaxError error("expected either ':' or '...'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    if(rule)
        rule->name = name;
    return rule;
}

};
