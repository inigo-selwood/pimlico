#pragma once

#include <memory>
#include <string>
#include <vector>

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
            std::vector<TextBuffer::SyntaxError> &errors,
            const unsigned int parent_count);

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
        std::vector<TextBuffer::SyntaxError> &errors,
        const unsigned int parent_count = 0) {

    TextBuffer::Position position = buffer.position;

    // Parse the rule's name
    std::string name;
    while(true) {
        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            name += buffer.read();
        else
            break;
    }

    // Check a name was found
    if(name.empty())
        throw ParseLogicError("no rule found", buffer);

    // Handle 'normal' rules
    buffer.skip_space();
    if(buffer.read(':')) {

        // Parse the rule's value
        buffer.skip_space(true);
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            buffer.skip_line(true);
            return nullptr;
        }

        // Create the rule
        std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());
        rule->name = name;
        rule->position = position;
        rule->terminal = true;
        rule->value = term;

        return rule;
    }

    // Handle name-extended rules
    else if(buffer.read("...")) {

        // Check there weren't trailing characters
        buffer.skip_space(true);
        if(buffer.peek('\n') == false) {
            const std::string message =
                    "trailing characters after name extended rule";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Parse children (rules defined below this one, indented by 4 spaces)
        std::vector<std::shared_ptr<Rule>> children;
        bool errors_found = false;
        while(true) {

            // Check if the next line is a single-indented child
            const int indentation_delta =
                    buffer.indentation_delta(position.line);
            if(indentation_delta <= 0)
                break;
            else
                buffer.skip_whitespace();

            // Parse the child
            const auto child = Rule::parse(buffer, errors, parent_count + 1);
            if(child == nullptr) {
                buffer.skip_line(true);
                errors_found = true;
            }
            else if(buffer.end_reached() == false && buffer.peek('\n') == false)
                throw ParseLogicError("incomplete rule parse", buffer);

            // Scope the child appropriately and add it to the vector
            else {
                child->add_parent_scope(name);
                children.push_back(child);
            }
        }

        if(errors_found)
            return nullptr;

        // Check there were children found
        else if(children.empty()) {
            buffer.position = position;
            const std::string message = "no children found for name-extended "
                    "rule '" + name + "'";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Create the rule
        std::shared_ptr<Rule> rule = std::shared_ptr<Rule>(new Rule());
        rule->name = name;
        rule->terminal = false;
        rule->position = position;
        rule->value = children;

        return rule;
    }

    else {
        const TextBuffer::SyntaxError error("expected ':' or '...'", buffer);
        errors.push_back(error);
        buffer.skip_line(true);
        return nullptr;
    }
}

};
