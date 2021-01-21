#pragma once

#include <array>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "syntax_error.hpp"
#include "text_buffer.hpp"

class Rule;

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
            std::shared_ptr<Rule>,
            std::vector<std::shared_ptr<Term>>> value;

    std::array<int, 2> instance_range;

    Type type;

    Predicate predicate;

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

private:

    static int parse_integer(TextBuffer &buffer);

    static Predicate parse_predicate(TextBuffer &buffer);
    static std::array<int, 2> parse_instance_range(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

// Helper for parsing values in term instance ranges
int Term::parse_integer(TextBuffer &buffer) {
    std::string text;
    while(true) {
        if(buffer.end_reached())
            break;

        const char character = buffer.peek();
        if(character >= '0' && character <= '9')
            text += buffer.read();
        else
            break;
    }

    if(text.empty())
        throw;

    try {
        return std::stoi(text);
    }
    catch(...) {
        return -1;
    }
}

// Helper for parsing predicates
Term::Predicate Term::parse_predicate(TextBuffer &buffer) {
    if(buffer.read('&'))
        return Predicate::AND;
    else if(buffer.read('!'))
        return Predicate::NOT;
    else
        return Predicate::NONE;
}

// Helper for parsing instance ranges
std::array<int, 2> Term::parse_instance_range(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    // Parse instance hints
    if(buffer.read('?'))
        return {0, 1};
    else if(buffer.read('*'))
        return {0, -1};
    else if(buffer.read('+'))
        return {1, -1};

    // Parse instance ranges
    else if(buffer.read('{')) {
        buffer.skip_space();

        // Parse start value
        const char start_character = buffer.peek();
        int start_value = -1;
        if(start_character >= '0' || start_character <= '9') {
            start_value = parse_integer(buffer);
            if(start_value == -1) {
                const SyntaxError error("invalid value in instance range",
                        buffer);
                errors.push_back(error);
                return {0, 0};
            }
        }

        // Check semi-colon present
        buffer.skip_space();
        if(buffer.read(':') == false) {
            const SyntaxError error("':' expected in instance range", buffer);
            errors.push_back(error);
            return {0, 0};
        }

        // Parse end value
        buffer.skip_space();
        const char end_character = buffer.peek();
        int end_value = -1;
        if(end_character >= '0' || end_character <= '9') {
            end_value = parse_integer(buffer);
            if(end_value == -1) {
                const SyntaxError error("invalid end value in instance range",
                        buffer);
                errors.push_back(error);
                return {0, 0};
            }
        }

        // Check closing bracket present
        buffer.skip_space();
        if(buffer.read('}') == false) {
            const SyntaxError error("'}' expected in instance range", buffer);
            errors.push_back(error);
            return {0, 0};
        }

        // Check at least one bound specified
        if(start_value == -1 && end_value == -1) {
            const SyntaxError error("no start/end values in instance range",
                    buffer);
            errors.push_back(error);
            return {0, 0};
        }

        else if(start_value == 0 && end_value == 0) {
            const SyntaxError error("redundant zero-instance term", buffer);
            errors.push_back(error);
            return {0, 0};
        }

        return {start_value, end_value};
    }

    // Default
    else
        return {1, 1};
}

std::shared_ptr<Term> Term::parse(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    const Predicate predicate = parse_predicate(buffer);

    std::shared_ptr<Term> term = nullptr;


    const std::array<int, 2> instance_range = parse_instance_range(buffer, errors);
    if(instance_range == std::array<int, 2>({0, 0}))
        return nullptr;

    // term->predicate = predicate;
    // term->instance_range = instance_range;

    return term;
}

};
