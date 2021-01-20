#pragma once

#include <array>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

namespace Pimlico {

class Term {

public:

    enum Type {
        CONSTANT,
        RANGE,
        REFERENCE,

        CHOICE,
        SEQUENCE,
    };

    enum Predicate {
        AND,
        NOT,

        NONE,
    };

    std::variant<std::string,
            std::array<char, 2>,
            std::shared_ptr<Term>,
            std::vector<std::shared_ptr<Term>>> value;

    std::array<int, 2> instance_hint;

    Type type;

    Predicate predicate;

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

private:

    static std::shared_ptr<Term> parse_constant(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static std::shared_ptr<Term> parse_range(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static std::shared_ptr<Term> parse_reference(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

std::shared_ptr<Term> Term::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    
}

};
