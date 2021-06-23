#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "parse_logic_error.hpp"
#include "text_buffer.hpp"

#include "rule.hpp"

namespace Pimlico {

class Specification {

public:

    std::map<unsigned int, std::shared_ptr<Rule>> rule_hashes;

    std::vector<std::shared_ptr<Rule>> rules;

    friend std::ostream &operator<<(std::ostream &stream,
            const Specification &specification);

    static std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<TextBuffer::SyntaxError> &errors);

private:

    static inline bool skip_comment(TextBuffer &buffer);

    static bool hash_rules(
            const std::vector<std::shared_ptr<Rule>> &rules,
            std::map<unsigned int, std::shared_ptr<Rule>> &rule_hashes,
            const TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);

};

// Serialize specification
std::ostream &operator<<(std::ostream &stream,
        const Specification &specification) {

    for(const auto &rule : specification.rules)
        stream << *rule << "\n";
    return stream;
}

/* Parses a grammar specification

Arguments:
    grammar (const std::string &): the grammar specification
    errors (std::vector<TextBuffer::SyntaxError &): a vector of errors to store
        accumulated errors in

Returns:
    specification (std:shared_ptr<Specification>): the resultant specification,
        or nullptr if an error occured during parsing
*/
std::shared_ptr<Specification> Specification::parse(const std::string &grammar,
        std::vector<TextBuffer::SyntaxError> &errors) {

    // Create a buffer
    TextBuffer buffer(grammar);
    buffer.comment_skip_function = skip_comment;

    // Check the buffer's text is valid
    if(buffer.valid(errors) == false)
        return nullptr;

    // Create a specification
    std::shared_ptr<Specification> specification =
            std::shared_ptr<Specification>(new Specification());

    // Parse the grammar's rules
    bool errors_found = false;
    while(true) {
        buffer.skip_whitespace();
        if(buffer.end_reached())
            break;

        // Parse an (expected) rule
        const auto rule = Rule::parse(buffer, errors);
        if(rule == nullptr) {
            if(buffer.end_reached() == false && buffer.peek('\n') == false)
                throw ParseLogicError("incomplete rule parse", buffer);
            errors_found = true;
        }
        else
            specification->rules.push_back(rule);
    }

    if(errors_found)
        return nullptr;

    // Place the rules in the hash map, checking for duplicates
    else if(hash_rules(specification->rules,
            specification->rule_hashes,
            buffer,
            errors) == false)
        return nullptr;

    // Resolve rules' terms' references, checking for errors
    bool references_valid = true;
    for(const auto &rule : specification->rules) {
        references_valid &= rule->resolve_references(specification->rule_hashes,
                buffer,
                errors);
    }
    if(references_valid == false)
        return nullptr;

    return specification;
}

// Skip comments
inline bool Specification::skip_comment(TextBuffer &buffer) {
    if(buffer.read('#') == false)
        return false;

    buffer.skip_line();
    return true;
}

/* Adds a vector of rules' name hashes to a map

Arguments:
    rules (const std::vector<std::shared_ptr<Rule>>): the rules to hash and add
        to the map
    rule_hashes (std::map<unsigned int, std::shared_ptr<Rule>>): the map of
        hashes to add the rules to
    buffer (const TextBuffer &): text buffer (used for error reporting)
    errors (std::vector<TextBuffer::SyntaxError> &): list of errors to add
        duplicates to

Returns:
    valid (bool): true if there were no duplicates detected
*/
bool Specification::hash_rules(
        const std::vector<std::shared_ptr<Rule>> &rules,
        std::map<unsigned int, std::shared_ptr<Rule>> &rule_hashes,
        const TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    bool result = true;
    for(const auto &rule : rules) {
        std::string full_path = rule->name;
        for(const auto &parent : rule->path)
            full_path += "_" + parent;
        const unsigned int hash = std::hash<std::string>{}(full_path);

        // Check the rule wasn't already in the map
        if(rule_hashes[hash]) {
            TextBuffer::SyntaxError error;

            error.add_reference("duplicate of rule '" + rule->name + "'",
                    buffer,
                    &rule->position);
            error.add_reference("first defined here:",
                    buffer,
                    &rule_hashes[hash]->position);

            errors.push_back(error);
            result = false;
        }
        else
            rule_hashes[hash] = rule;

        // If the rule is non-terminal, check its children for duplicates
        if(rule->terminal == false) {
            const std::vector<std::shared_ptr<Rule>> children =
                    std::get<std::vector<std::shared_ptr<Rule>>>(rule->value);
            result &= hash_rules(children, rule_hashes, buffer, errors);
        }
    }

    return result;
}

};
