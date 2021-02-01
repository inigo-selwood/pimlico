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

private:

    std::vector<std::shared_ptr<Rule>> rules;

    static inline bool skip_comment(TextBuffer &buffer);

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Specification &specification);

    static std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<TextBuffer::SyntaxError> &errors);

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

    return specification;
}

};
