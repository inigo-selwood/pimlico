#pragma once

#include <array>
#include <map>
#include <memory>
#include <ostream>
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

    friend std::ostream &operator<<(std::ostream &stream,
                const std::weak_ptr<Term> &term);

    std::variant<std::string,
            std::array<char, 2>,
            std::vector<std::weak_ptr<Term>>> value;

    std::array<int, 2> instance_bounds;

    Type type;

    Predicate predicate;

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors,
            const bool root);

private:

    std::vector<std::shared_ptr<Term>> unique_terms;

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

std::ostream &operator<<(std::ostream &stream,
            const std::weak_ptr<Term> &term) {
    static std::map<Term::Type, std::string> type_names = {
        {Term::Type::CONSTANT,  "constant"},
        {Term::Type::RANGE,     "range"},
        {Term::Type::REFERENCE, "reference"},

        {Term::Type::CHOICE,    "choice"},
        {Term::Type::SEQUENCE,  "sequence"}
    };

    std::shared_ptr<Term> pointer = term.lock();
    if(pointer == nullptr)
        return stream;

    stream << "(" << type_names[pointer->type] << " | ";

    // Print constants/reference names
    if(pointer->type == Term::Type::CONSTANT
            || pointer->type == Term::Type::REFERENCE)
        stream << std::get<std::string>(pointer->value);

    // Print ranges
    else if(pointer->type == Term::Type::RANGE) {
        const auto range = std::get<std::array<char, 2>>(pointer->value);
        if(range[0] > 0)
            stream << range[0] << " ";

        stream << ":";

        if(range[1] > 0)
            stream << " " << range[1];
    }

    // Print choices/sequences
    else if(pointer->type == Term::Type::CHOICE
            || pointer->type == Term::Type::SEQUENCE) {
        const auto values =
                std::get<std::vector<std::weak_ptr<Term>>>(pointer->value);
        for(unsigned int index = 0; index < values.size(); index += 1) {
            stream << values[index];
            if(index + 1 < values.size())
                stream << ", ";
        }
    }

    stream << ")";
    return stream;
}

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

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());

    // If a term is the root of a rule, or is enclosed in parentheses, it should
    // assume it's a sequence
    bool line_broken = false;
    if(root || enclosed) {

        std::vector<std::shared_ptr<Term>> values;
        while(true) {
            buffer.skip_space();
            if(buffer.end_reached())
                break;

            // Continue parsing on the next line if it's double-indented
            if(buffer.peek('\n')) {
                const unsigned int next_indentation =
                        buffer.line_indentation(buffer.position.line_number);
                const int indentation_delta =
                        next_indentation - buffer.indentation();
                if((line_broken == false && indentation_delta == 0) ||
                        (line_broken && indentation_delta == 2)) {
                    buffer.increment();
                    continue;
                }
                else
                    break;
            }

            // Parse the next term in the sequence
            std::shared_ptr<Term> term = Term::parse(buffer, errors);
            if(term == nullptr)
                return nullptr;

            // If a choice symbol is encountered, stop to parse the choice term
            buffer.skip_space();
            if(buffer.peek('|')) {

                // Create a vector to hold the choice's values, with the
                // just-parsed term as its first option
                std::vector<std::shared_ptr<Term>> options;
                options.push_back(term);

                // Keep parsing options until no more choice symbols are found
                while(true) {
                    buffer.skip_space();
                    if(buffer.end_reached()) {
                        const SyntaxError error("unexpected end-of-file",
                                buffer);
                        errors.push_back(error);
                        return nullptr;
                    }

                    // Continue parsing on the next line if it's double-indented
                    else if(buffer.peek('\n')) {
                        const unsigned int line_number =
                                buffer.position.line_number;
                        const unsigned int next_indentation =
                                buffer.line_indentation(line_number);

                        const int indentation_delta =
                                next_indentation - buffer.indentation();
                        if((line_broken == false && indentation_delta == 0) ||
                                (line_broken && indentation_delta == 2)) {
                            buffer.increment();
                            continue;
                        }
                        else {
                            const SyntaxError error("unexpected end-of-line",
                                    buffer);
                            errors.push_back(error);
                            return nullptr;
                        }
                    }

                    if(buffer.read('|') == false)
                        break;

                    buffer.skip_space();
                    std::shared_ptr<Term> option = Term::parse(buffer, errors);
                    if(option == nullptr) {
                        const std::string error_message =
                                "expected term after choice operator '|'";
                        SyntaxError error(error_message, buffer);
                        errors.push_back(error);
                        return nullptr;
                    }
                    options.push_back(option);
                }

                // Create a choice term with the options parsed
                std::shared_ptr<Term> choice =
                        std::shared_ptr<Term>(new Term());
                choice->type = Type::CHOICE;
                choice->unique_terms = options;

                // choice->value = options;

                std::vector<std::weak_ptr<Term>> weak_options;
                for(const auto &option : options)
                    weak_options.push_back(option);
                choice->value = weak_options;

                values.push_back(choice);
            }

            values.push_back(term);
        }

        if(values.size() == 1)
            term = values.back();
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
