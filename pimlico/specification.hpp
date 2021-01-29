#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "parse_logic_error.hpp"
#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "rule.hpp"

namespace Pimlico {

class Specification {

private:

    std::vector<std::shared_ptr<Rule>> rules;

    static inline bool skip_comment(TextBuffer &buffer);

    static bool detect_duplicates(
            const std::vector<std::shared_ptr<Rule>> &rules,
            std::vector<SyntaxError> &errors,
            TextBuffer &buffer);

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Specification &specification);

    static std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<SyntaxError> &errors);

};

std::ostream &operator<<(std::ostream &stream,
        const Specification &specification) {

    for(const auto &rule : specification.rules)
        stream << *rule << "\n";
    return stream;
}

inline bool Specification::skip_comment(TextBuffer &buffer) {
    if(buffer.read('#') == false)
        return false;

    buffer.skip_line();
    return true;
}

bool Specification::detect_duplicates(
        const std::vector<std::shared_ptr<Rule>> &rules,
        std::vector<SyntaxError> &errors,
        TextBuffer &buffer) {

    bool duplicates_detected = false;

    std::vector<std::string> names;
    std::map<std::string, std::vector<TextBuffer::Position>> duplicates;
    for(const auto &rule : rules) {
        if(std::find(names.begin(), names.end(), rule->name) != names.end()) {
            duplicates[rule->name].push_back(rule->position);
            duplicates_detected = true;
        }
        else
            names.push_back(rule->name);

        if(rule->terminal == false) {
            const std::vector<std::shared_ptr<Rule>> children =
                    std::get<std::vector<std::shared_ptr<Rule>>>(rule->value);
            duplicates_detected |= detect_duplicates(children, errors, buffer);
        }
    }

    for(const auto &duplicate : duplicates) {
        for(const auto &position : duplicate.second) {
            buffer.position = position;
            std::string message =
                    "redefinition of rule '" + duplicate.first + "'";
            SyntaxError error(message, buffer);
            errors.push_back(error);
        }
    }

    return duplicates_detected;
}

std::shared_ptr<Specification> Specification::parse(const std::string &grammar,
        std::vector<SyntaxError> &errors) {

    TextBuffer buffer(grammar);
    buffer.comment_skip_function = skip_comment;

    std::shared_ptr<Specification> specification =
            std::shared_ptr<Specification>(new Specification());

    bool errors_found = false;
    while(true) {
        buffer.skip_whitespace();
        if(buffer.end_reached())
            break;

        const auto rule = Rule::parse(buffer, errors);
        if(rule == nullptr)
            errors_found = true;
        else
            specification->rules.push_back(rule);

        if(buffer.end_reached() == false && buffer.peek('\n') == false)
            throw ParseLogicError("incomplete rule parse", buffer);
    }

    if(detect_duplicates(specification->rules, errors, buffer) || errors_found)
        return nullptr;

    return specification;
}

};
