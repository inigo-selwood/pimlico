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

    TextBuffer::Position position;

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

    std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());

    rule->position = buffer.position;

    if(buffer.indentation() % 4) {
        SyntaxError error("invalid indentation level", buffer);
        errors.push_back(error);
        return nullptr;
    }

    while(true) {
        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            rule->name += buffer.read();
        else
            break;
    }

    if(rule->name.empty())
        throw ParseLogicError("no rule found", buffer);

    // Handle 'normal' rules
    buffer.skip_space();
    if(buffer.read(':')) {
        rule->terminal = true;

        buffer.skip_space(true);
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            buffer.skip_block();
            return nullptr;
        }

        rule->value = term;
    }

    // Handle name-extended rules
    else if(buffer.read("...")) {
        buffer.skip_space(true);
        bool errors_found = false;
        if(buffer.peek('\n') == false) {
            SyntaxError error("trailing characters after '...'",
                    buffer);
            errors.push_back(error);
            errors_found = true;
        }

        // Keep track of the start indentation
        const unsigned int start_indentation = buffer.indentation();

        // Parse children (rules defined below this one, indented by 4 spaces)
        std::vector<std::shared_ptr<Rule>> children;
        while(true) {

            // Check if the next line is a single-indented child
            const unsigned int new_indentation =
                    buffer.indentation(buffer.position.line_number);
            if(buffer.position.line_number >= buffer.line_indentations.size()
                    || new_indentation <= start_indentation)
                break;
            else
                buffer.skip_whitespace();

            // Parse the child
            const auto child = Rule::parse(buffer, errors);
            if(child == nullptr) {
                buffer.skip_block();
                errors_found = true;
            }
            else if(buffer.end_reached() == false && buffer.peek('\n') == false)
                throw ParseLogicError("incomplete rule parse", buffer);

            // Scope the child appropriately and add it to the vector
            else {
                child->add_parent_scope(rule->name);
                children.push_back(child);
            }
        }

        if(errors_found)
            return nullptr;
        else if(children.empty()) {
            buffer.position = rule->position;
            const std::string message = "no children found for name-extended "
                    "rule '" + rule->name + "'";
            SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        rule->value = children;
    }

    else {
        SyntaxError error("expected ':' or '...'", buffer);
        errors.push_back(error);
        buffer.skip_block();
        return nullptr;
    }

    return rule;
}

};
