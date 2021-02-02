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

    friend std::ostream &operator<<(std::ostream &stream,
            const Specification &specification);

    static std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<TextBuffer::SyntaxError> &errors);

private:

    std::vector<std::shared_ptr<Rule>> rules;

    static inline bool skip_comment(TextBuffer &buffer);

    static bool hash_rules(
            const std::vector<std::shared_ptr<Rule>> &rules,
            std::map<unsigned int, std::shared_ptr<Rule>> &rule_hashes,
            const TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);

};

std::ostream &operator<<(std::ostream &stream,
        const Specification &specification) {

    for(const auto &rule : specification.rules)
        stream << *rule << "\n";
    return stream;
}

std::shared_ptr<Specification> Specification::parse(const std::string &grammar,
        std::vector<TextBuffer::SyntaxError> &errors) {

    TextBuffer buffer(grammar);
    buffer.comment_skip_function = skip_comment;

    if(buffer.valid(errors) == false)
        return nullptr;

    std::shared_ptr<Specification> specification =
            std::shared_ptr<Specification>(new Specification());

    bool errors_found = false;
    while(true) {
        buffer.skip_whitespace();
        if(buffer.end_reached())
            break;

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
    else if(hash_rules(specification->rules,
            specification->rule_hashes,
            buffer,
            errors) == false)
        return nullptr;

    return specification;
}

inline bool Specification::skip_comment(TextBuffer &buffer) {
    if(buffer.read('#') == false)
        return false;

    buffer.skip_line();
    return true;
}

bool Specification::hash_rules(
        const std::vector<std::shared_ptr<Rule>> &rules,
        std::map<unsigned int, std::shared_ptr<Rule>> &rule_hashes,
        const TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    bool result = true;
    for(const auto &rule : rules) {
        const unsigned int hash = std::hash<std::string>{}(rule->name);
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

        if(rule->terminal == false) {
            const std::vector<std::shared_ptr<Rule>> children =
                    std::get<std::vector<std::shared_ptr<Rule>>>(rule->value);
            result &= hash_rules(children, rule_hashes, buffer, errors);
        }
    }

    return result;
}

};
