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
    static char parse_escape_code(TextBuffer &buffer);
    static bool skip_line_extension(TextBuffer &buffer, bool &line_broken);

    static inline std::array<int, 2> parse_instance_bounds(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

    static inline std::array<char, 2> parse_range(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static inline std::string parse_constant(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

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

    if(pointer->instance_bounds[0] != 1 || pointer->instance_bounds[1] != 1) {
        stream << "<" << pointer->instance_bounds[0] << ":" << pointer->instance_bounds[1] << ">";
    }

    return stream;
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

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());

    // Check if enclosed in parentheses (ignore if already root to prevent
    // premature cessation of parsing)
    bool enclosed = false;
    if(root == false && buffer.read('('))
        enclosed = true;

    // If a term is the root of a rule, or is enclosed in parentheses, it should
    // assume it's a sequence
    bool line_broken = false;
    if(root || enclosed) {
        term->type = Type::SEQUENCE;

        std::vector<std::shared_ptr<Term>> values;

        while(true) {

            // Parse the next term in the sequence
            std::shared_ptr<Term> term = Term::parse(buffer, errors);
            if(term == nullptr)
                return nullptr;
            values.push_back(term);

            // Continue parsing on the next line if it's double-indented
            buffer.skip_space();
            if(buffer.end_reached())
                break;
            else if(buffer.peek('\n')) {
                if(skip_line_extension(buffer, line_broken) == false)
                    break;
            }

            // If a choice symbol is encountered, stop to parse the choice term
            if(buffer.peek('|')) {

                // Create a vector to hold the choice's values, with the
                // just-parsed term as its first option
                std::vector<std::shared_ptr<Term>> options;
                options.push_back(term);
                values.pop_back();

                // Keep parsing options until no more choice symbols are found
                while(true) {
                    if(buffer.read('|') == false)
                        break;

                    buffer.skip_space();
                    if(buffer.end_reached()) {
                        const SyntaxError error("unexpected end-of-file",
                                buffer);
                        errors.push_back(error);
                        return nullptr;
                    }

                    // Continue parsing on the next line if it's double-indented
                    else if(buffer.peek('\n')) {
                        if(skip_line_extension(buffer, line_broken) == false) {
                            const SyntaxError error("unexpected end-of-line",
                                    buffer);
                            errors.push_back(error);
                            return nullptr;
                        }
                    }

                    buffer.skip_space();
                    std::shared_ptr<Term> option = Term::parse(buffer, errors);
                    if(option == nullptr)
                        return nullptr;
                    options.push_back(option);

                    // Continue parsing on the next line if it's double-indented
                    buffer.skip_space();
                    if(buffer.end_reached())
                        break;
                    else if(buffer.peek('\n')) {
                        if(skip_line_extension(buffer, line_broken) == false)
                            break;
                    }
                }

                // Create a choice term with the options parsed
                std::shared_ptr<Term> choice =
                        std::shared_ptr<Term>(new Term());
                choice->type = Type::CHOICE;
                choice->unique_terms = options;
                choice->predicate = Predicate::NONE;
                choice->instance_bounds = {1, 1};

                std::vector<std::weak_ptr<Term>> weak_options;
                for(const auto &option : options)
                    weak_options.push_back(option);
                choice->value = weak_options;

                values.push_back(choice);
            }

            if(buffer.peek(')'))
                break;
        }

        if(values.size() == 0) {
            const SyntaxError error("empty statement", buffer);
            errors.push_back(error);
            return nullptr;
        }
        else if(values.size() == 1)
            term = values.back();
        else {
            term->unique_terms = values;

            std::vector<std::weak_ptr<Term>> weak_values;
            for(const auto &value : values)
                weak_values.push_back(value);
            term->value = weak_values;
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

                const char letter = buffer.peek();
                if((letter >= 'a' && letter <= 'z') || letter == '_')
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

    term->instance_bounds = instance_bounds;
    term->predicate = predicate;
    return term;
}

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
        throw "spartan";

    try {
        return std::stoi(text);
    }
    catch(...) {
        return -1;
    }
}

char Term::parse_escape_code(TextBuffer &buffer) {

    const TextBuffer::Position start_position = buffer.position;

    if(buffer.read('\\') == false)
        throw;

    switch(buffer.read()) {
        case '\'':
            return '\'';
        case '\"':
            return '\"';
        case '\\':
            return '\\';
        case 'b':
            return '\b';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        default:
            buffer.position = start_position;
            return 0;
    }
}

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
        if(start_character >= '0' && start_character <= '9') {
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

inline std::string Term::parse_constant(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    const TextBuffer::Position start_position = buffer.position;

    if(buffer.read('\'') == false)
        throw;

    std::string value;
    while(true) {
        if(buffer.end_reached()) {
            SyntaxError error("unexpected end-of-file", buffer);
            errors.push_back(error);
            return "";
        }
        else if(buffer.peek('\n')) {
            SyntaxError error("unexpected end-of-line", buffer);
            errors.push_back(error);
            return "";
        }
        else if(buffer.peek('\''))
            break;

        // Handle escape codes
        else if(buffer.peek('\\')) {
            const char escape_code = parse_escape_code(buffer);
            if(escape_code == 0) {
                SyntaxError error("invalid escape code", buffer);
                errors.push_back(error);
                return "";
            }

            value += escape_code;
        }
        else
            value += buffer.read();
    }

    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return "";
    }

    if(value.empty()) {
        buffer.position = start_position;
        SyntaxError error("empty constant", buffer);
        errors.push_back(error);
        return "";
    }

    return value;
}

inline std::array<char, 2> Term::parse_range(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    if(buffer.read('[') == false)
        throw;

    buffer.skip_space();
    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    const char start_value = buffer.read();
    if(start_value < ' ' || start_value > '~') {
        SyntaxError error("start constant must be a valid ASCII character",
                buffer);
        errors.push_back(error);
        return {0, 0};
    }

    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    buffer.skip_space();
    if(buffer.read('-') == false) {
        SyntaxError error("expected '-'", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    buffer.skip_space();
    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    const char end_value = buffer.read();
    if(end_value < ' ' || end_value > '~') {
        SyntaxError error("end constant must be a valid ASCII character",
                buffer);
        errors.push_back(error);
        return {0, 0};
    }

    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    buffer.skip_space();
    if(buffer.read(']') == false) {
        SyntaxError error("expected ']'", buffer);
        errors.push_back(error);
        return {0, 0};
    }

    return {start_value, end_value};
}

bool Term::skip_line_extension(TextBuffer &buffer, bool &line_broken) {
    const unsigned int next_indentation =
            buffer.line_indentation(buffer.position.line_number + 1);
    const int indentation_delta = next_indentation - buffer.indentation();
    if((line_broken == false && indentation_delta == 8) ||
            (line_broken && indentation_delta >= 0)) {
        buffer.skip_whitespace();
        line_broken = true;
        return true;
    }

    return false;
}

};
