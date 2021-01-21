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
            std::vector<std::shared_ptr<Term>>> value;

    std::array<int, 2> instance_bounds;

    Type type;

    Predicate predicate;

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors,
            const bool root);

private:

    static int parse_integer(TextBuffer &buffer);

    static inline std::array<int, 2> parse_instance_bounds(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

    static inline std::array<char, 2> parse_range(TextBuffer &buffer,
            std::vector<SyntaxError> &errors) {
        return std::array<char, 2>({0, 0});
    }

    static inline std::string parse_constant(TextBuffer &buffer,
            std::vector<SyntaxError> &errors) {
         return "";
    }

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

// Helper for parsing instance ranges
inline std::array<int, 2> Term::parse_instance_bounds(TextBuffer &buffer,
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
        std::vector<SyntaxError> &errors,
        const bool root = false) {

    // Parse predicate
    Predicate predicate = Predicate::NONE;
    if(buffer.read('&'))
        predicate = Predicate::AND;
    else if(buffer.read('!'))
        predicate = Predicate::NOT;

    // Check if enclosed in parentheses
    bool enclosed = false;
    if(buffer.read('('))
        enclosed = true;

    const auto term = std::shared_ptr<Term>(new Term());

    // If a term is the root of a rule, or is enclosed in parentheses, it should
    // assume it's a sequence
    if(root || enclosed) {
        while(true) {
            if(buffer.end_reached())
                break;


        }
    }

    else {

        const char character = buffer.peek();

        // Parse constants
        if(character == '\'') {
            term->type = Type::CONSTANT;

            const std::string value = parse_constant(buffer, errors);
            if(value.empty())
                return nullptr;
            term->value = value;
        }

        // Parse ranges
        else if(character == '[') {
            term->type = Type::RANGE;

            const std::array<char, 2> value = parse_range(buffer, errors);
            if(value == std::array<char, 2>({0, 0}))
                return nullptr;
            term->value = value;
        }

        // Handle references
        else if((character >= 'a' && character <= 'z') || character == '_') {
            term->type = Type::REFERENCE;

            std::string value;
            while(true) {
                if(buffer.end_reached())
                    break;

                const auto letter = buffer.peek();
                if((character >= 'a' && character <= 'z') || character == '_')
                    value += buffer.read();
                else
                    break;
            }
            term->value = value;
        }

        else {
            const SyntaxError error("expected a term", buffer);
            errors.push_back(error);
            return nullptr;
        }
    }

    // Check for a matching closing parenthesis if one is expected
    if(enclosed && buffer.read(')') == false) {
        const SyntaxError error("expected ')'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Parse instance range
    const std::array<int, 2> instance_bounds =
            parse_instance_bounds(buffer, errors);
    if(instance_bounds == std::array<int, 2>({0, 0}))
        return nullptr;

    return term;
}

};
