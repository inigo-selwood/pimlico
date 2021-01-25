#pragma once

#include <memory>
#include <vector>
#include <string>

#include "parse_logic_error.hpp"
#include "syntax_error.hpp"
#include "text_buffer.hpp"

#include "rule.hpp"

namespace Pimlico {

class Specification {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Specification &specification);

    static std::shared_ptr<Specification> parse(const std::string &grammar,
            std::vector<SyntaxError> &errors);

private:

    std::vector<std::shared_ptr<Rule>> rules;

    static inline bool skip_comment(TextBuffer &buffer);

};

std::ostream &operator<<(std::ostream &stream,
        const Specification &specification) {

    for(const auto &rule : specification.rules)
        stream << *rule << "\n";
    return stream;
}

std::shared_ptr<Specification> Specification::parse(const std::string &grammar,
        std::vector<SyntaxError> &errors) {

    TextBuffer buffer(grammar);
    buffer.comment_skip_function = skip_comment;

    std::shared_ptr<Specification> specification =
            std::shared_ptr<Specification>(new Specification());

    while(true) {
        buffer.skip_whitespace();
        if(buffer.end_reached())
            break;

        const std::shared_ptr<Rule> rule = Rule::parse(buffer, errors);
        if(buffer.end_reached() == false && buffer.read('\n') == false)
            throw ParseLogicError("incomplete rule parse", buffer);
        else if(rule == nullptr)
            specification = nullptr;
        else if(specification)
            specification->rules.push_back(rule);
    }

    buffer.skip_whitespace();
    if(buffer.end_reached() == false)
        throw ParseLogicError("incomplete specification parse", buffer);

    return specification;
}

inline bool Specification::skip_comment(TextBuffer &buffer) {
    if(buffer.read('#') == false)
        return false;

    buffer.skip_line();
    return true;
}

};
