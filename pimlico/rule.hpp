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

    std::vector<std::string> path;

    std::string name;

    bool terminal;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule);

    static std::shared_ptr<Rule> parse(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors,
            const unsigned int parent_count);

    bool resolve_references(
            std::map<unsigned int, std::shared_ptr<Rule>> &rules,
            const TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);

    std::shared_ptr<Term> term_value();

};

// Recursively adds a parent scope to this rule and its children
void Rule::add_parent_scope(const std::string &parent) {
    path.push_back(parent);
    if(terminal == false) {
        const std::vector<std::shared_ptr<Rule>> &children =
                std::get<std::vector<std::shared_ptr<Rule>>>(value);
        for(const std::shared_ptr<Rule> &child : children)
            child->add_parent_scope(parent);
    }
}

std::ostream &operator<<(std::ostream &stream, const Rule &rule) {

    // Indent the rule
    for(unsigned int index = 0; index < rule.path.size(); index += 1)
        stream << "    ";

    // Serialize terminal rules
    if(rule.terminal) {
        const std::shared_ptr<Term> &value =
                std::get<std::shared_ptr<Term>>(rule.value);
        stream << rule.name << ": " << *value;
    }

    // Serialize non-terminal rules (ie: rules with children)
    else {
        const std::vector<std::shared_ptr<Rule>> &children =
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

    // Check the indentation level is correct
    if(buffer.line_indentation() != parent_count * 4) {
        const std::string message = "unexpected indentation increase";
        const TextBuffer::SyntaxError error(message, buffer);
        errors.push_back(error);
        buffer.skip_line(true);
        return nullptr;
    }

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

        // Parse children (rules defined below this one, indented by 4
        // spaces)
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
            const auto child = Rule::parse(buffer,
                    errors,
                    parent_count + 1);
            if(child == nullptr) {
                buffer.skip_line(true);
                errors_found = true;
            }
            else if(buffer.end_reached() == false
                    && buffer.peek('\n') == false)
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
            const std::string message =
                    "no children found for name-extended rule '" +
                            name + "'";
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
        const TextBuffer::SyntaxError error("expected ':' or '...'",
                buffer);
        errors.push_back(error);
        buffer.skip_line(true);
        return nullptr;
    }
}

bool Rule::resolve_references(
        std::map<unsigned int, std::shared_ptr<Rule>> &rules,
        const TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    bool result = true;

    // If the rule isn't terminal, resolve its children's references
    if(terminal == false) {
        const std::vector<std::shared_ptr<Rule>> &children =
                std::get<std::vector<std::shared_ptr<Rule>>>(value);
        for(const auto &child : children)
            result &= child->resolve_references(rules, buffer, errors);
        return result;
    }

    const std::shared_ptr<Term> &root = std::get<std::shared_ptr<Term>>(value);
    std::vector<std::shared_ptr<Term>> stack = {root};
    while(true) {
        if(stack.empty())
            break;

        // Handle sequences and choices
        auto &term = stack.back();
        if(term->type == Term::Type::CHOICE
                || term->type == Term::Type::SEQUENCE) {

            // Remove the term itself
            stack.pop_back();

            // Add its children to the stack
            const std::vector<std::shared_ptr<Term>> &children =
                    std::get<std::vector<std::shared_ptr<Term>>>(term->value);
            for(const auto &child : children)
                stack.push_back(child);
        }

        // Handle references
        else if(term->type == Term::Type::REFERENCE) {

            // Check whether the reference exists when suffixed with any of its
            // parents' names
            std::vector<std::shared_ptr<Rule>> candidates;
            const unsigned int parent_count = this->path.size();
            Term::Reference &reference = std::get<Term::Reference>(term->value);
            std::string test_path = reference.name;

            for(unsigned int index = 0; index < parent_count; index += 1) {

                const unsigned int hash = std::hash<std::string>{}(test_path);
                const std::shared_ptr<Rule> candidate = rules[hash];
                if(candidate)
                    candidates.push_back(candidate);

                test_path += "_" + this->path[index];
            }

            // Handle unmatched references
            if(candidates.size() == 0) {
                TextBuffer::SyntaxError error("no matches for reference",
                        buffer,
                        &term->position);
                errors.push_back(error);
                result = false;
            }

            // Handle ambiguous references with muliple matches
            else if(candidates.size() > 1) {
                TextBuffer::SyntaxError error;

                error.add_reference("multiple candidates for reference",
                        buffer,
                        &term->position);

                for(const auto &candidate : candidates) {
                    error.add_reference("potential candidate",
                            buffer,
                            &candidate->position);
                }

                errors.push_back(error);
                result = false;
            }

            // Replace the reference with the rule it's referencing's value
            else
                reference.value = candidates.back()->term_value();

            stack.pop_back();
        }

        // Ignore the term otherwise
        else
            stack.pop_back();
    }

    return result;
}

std::shared_ptr<Term> Rule::term_value() {

    // If terminal, just return the single value
    if(terminal)
        return std::get<std::shared_ptr<Term>>(value);

    // Place children's value into a vector of terms
    std::vector<std::shared_ptr<Term>> options;
    const std::vector<std::shared_ptr<Rule>> &children =
            std::get<std::vector<std::shared_ptr<Rule>>>(value);
    for(const auto &child : children)
        options.push_back(child->term_value());

    // Create a choice term that references this rule's position, with its
    // children's term values as options
    std::shared_ptr<Term> choice = std::shared_ptr<Term>(new Term());
    choice->type = Term::Type::CHOICE;
    choice->position = position;
    choice->value = options;

    return choice;
}

};
