#pragma once

#include <array>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "syntax_error.hpp"
#include "parse_logic_error.hpp"
#include "text_buffer.hpp"

namespace Pimlico {

static int parse_integer(TextBuffer &buffer) {

    // Keep start index for error reporting
    const TextBuffer::Position start_position = buffer.position;

    // Extract digits
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

    // Check digits were found
    if(text.empty()) {
        buffer.position = start_position;
        throw ParseLogicError("parse_integer called with no integer", buffer);
    }

    // Parse and return integer
    try {
        return std::stoi(text);
    }
    catch(...) {
        return -1;
    }
}

static char parse_escape_code(TextBuffer &buffer) {

    // Keep start index for error reporting
    const TextBuffer::Position start_position = buffer.position;

    // Check escape character present
    if(buffer.read('\\') == false)
        throw ParseLogicError("parse_escape_code called with no code", buffer);

    // Extract character
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
            return 0;
    }
}

class Term {

public:

    enum class Type {
        CONSTANT,
        RANGE,
        REFERENCE,

        CHOICE,
        SEQUENCE,

        NONE,
    };

    enum class Predicate {
        AND,
        NOT,

        NONE,
    };

    friend std::ostream &operator<<(std::ostream &stream, const Term &term);

    std::variant<std::string,
            std::array<char, 2>,
            std::vector<std::shared_ptr<Term>>> value;

    std::array<int, 2> instance_bounds;

    Type type;

    Predicate predicate;

    bool silenced;

    Term() : instance_bounds({0, 0}),
            type(Type::NONE),
            predicate(Predicate::NONE),
            silenced(false) {}

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<SyntaxError> &errors,
            const bool root);

private:

    static std::shared_ptr<Term> parse_constant(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static std::shared_ptr<Term> parse_range(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static std::shared_ptr<Term> parse_reference(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

    static std::shared_ptr<Term> parse_choice(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);
    static std::shared_ptr<Term> parse_sequence(TextBuffer &buffer,
            std::vector<SyntaxError> &errors);

};

std::ostream &operator<<(std::ostream &stream, const Term &term) {

    // Serialize predicate
    switch(term.predicate) {
        case Term::Predicate::AND:
            stream << "&";
            break;
        case Term::Predicate::NOT:
            stream << "!";
            break;
    }

    static std::map<char, std::string> escape_codes = {
        {'\n', "\\n"},
        {'\r', "\\r"} ,
        {'\b', "\\b"},
        {'\t', "\\t"},
        {'\\', "\\\\"},
        {'\"', "\\\""},
        {'\'', "\\\'"},
    };

    if(term.silenced)
        stream << "$";

    // Serialize constants
    const Term::Type &type = term.type;
    if(type == Term::Type::CONSTANT) {
        stream << "'";
        for(const char &character : std::get<std::string>(term.value)) {
            if(escape_codes.find(character) != escape_codes.end())
                stream << escape_codes[character];
            else
                stream << character;
        }
        stream << "'";
    }

    // Serialize ranges
    else if(type == Term::Type::RANGE) {
        const std::array<char, 2> range =
                std::get<std::array<char, 2>>(term.value);

        stream << "['";

        if(escape_codes.find(range[0]) != escape_codes.end())
            stream << escape_codes[range[0]];
        else
            stream << range[0];

        stream << "' - '";

        if(escape_codes.find(range[1]) != escape_codes.end())
            stream << escape_codes[range[1]];
        else
            stream << range[1];

        stream << "']";
    }

    // Serialize references
    else if(type == Term::Type::REFERENCE)
        stream << std::get<std::string>(term.value);

    // Serialize choices and sequences
    else if(type == Term::Type::CHOICE || type == Term::Type::SEQUENCE) {
        const auto values =
                std::get<std::vector<std::shared_ptr<Term>>>(term.value);

        for(unsigned int index = 0; index < values.size(); index += 1) {
            const auto &pointer = values[index];
            const auto value_type = pointer->type;

            // Enclose the term if it's within one a container of the other type
            bool enclosed = false;
            if(type == Term::Type::CHOICE && value_type == Term::Type::SEQUENCE)
                enclosed = true;

            if(enclosed)
                stream << "(";

            stream << *pointer;

            if(enclosed)
                stream << ")";

            if(index + 1 < values.size()) {
                if(type == Term::Type::CHOICE)
                    stream << " | ";
                else if(type == Term::Type::SEQUENCE)
                    stream << " ";
            }
        }
    }

    // Extract bounds
    const std::array<int, 2> instance_bounds = term.instance_bounds;
    const int lower_bound = instance_bounds[0];
    const int upper_bound = instance_bounds[1];

    // Don't print anything more if the bounds are default
    if(lower_bound == upper_bound && lower_bound == 1)
        return stream;

    // Serialize instance hints
    else if(lower_bound == 0 && upper_bound == 1)
        stream << "?";
    else if(lower_bound == 0 && upper_bound == -1)
        stream << "*";
    else if(lower_bound == 1 && upper_bound == -1)
        stream << "+";

    // Serialize instance bounds
    else if(lower_bound == upper_bound)
        stream << "{" << lower_bound << "}";
    else if(lower_bound == -1)
        stream << "{:" << upper_bound << "}";
    else if(upper_bound == -1)
        stream << "{" << lower_bound << ":}";
    else
        stream << "{" << lower_bound << " : " << upper_bound << "}";

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

    buffer.skip_space();
    bool silenced = false;
    if(buffer.read('$')) {
        if(predicate != Predicate::NONE) {
            const SyntaxError error("unneccessarily silenced predicated term",
                    buffer);
            errors.push_back(error);
            return nullptr;
        }

        silenced = true;
    }

    buffer.skip_space();
    bool enclosed = false;
    if(root == false && buffer.read('('))
        enclosed = true;

    std::shared_ptr<Term> term = nullptr;

    if(root || enclosed)
        term = parse_sequence(buffer, errors);
    else {
        const char character = buffer.peek();

        if(character == '\'')
            term = parse_constant(buffer, errors);
        else if(character == '[')
            term = parse_range(buffer, errors);
        else if((character >= 'a' && character <= 'z') || character == '_')
            term = parse_reference(buffer, errors);
        else {
            const std::string message =
                    "invalid term: expected a constant, range, reference, or "
                    "sequence";
            const SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }
    }

    if(term == nullptr)
        return nullptr;

    buffer.skip_space();
    if(enclosed && buffer.read(')') == false) {
        const SyntaxError error("expected matching ')'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Parse instance hints
    buffer.skip_space();
    std::array<int, 2> instance_bounds;
    if(buffer.read('?'))
        instance_bounds = {0, 1};
    else if(buffer.read('*'))
        instance_bounds = {0, -1};
    else if(buffer.read('+'))
        instance_bounds = {1, -1};

    // Parse instance ranges
    else if(buffer.read('{')) {

        const TextBuffer::Position start_position = buffer.position;

        buffer.skip_space();
        int start_value = -1;
        char character = buffer.peek();
        if(character >= '0' && character <= '9') {
            start_value = parse_integer(buffer);
            if(start_value == -1) {
                SyntaxError error("invalid instance bound start value", buffer);
                errors.push_back(error);
                return nullptr;
            }
        }

        buffer.skip_space();
        bool colon_present = false;
        if(buffer.read(':'))
            colon_present = true;

        buffer.skip_space();
        int end_value = -1;
        character = buffer.peek();
        if(character >= '0' && character <= '9') {
            end_value = parse_integer(buffer);
            if(end_value == -1) {
                SyntaxError error("invalid instance bound end value", buffer);
                errors.push_back(error);
                return nullptr;
            }
        }

        buffer.skip_space();
        if(buffer.read('}') == false) {
            SyntaxError error("expected '}' at end of instance bound", buffer);
            errors.push_back(error);
            return nullptr;
        }

        // N instances
        if(start_value != -1 && end_value == -1 && colon_present == false) {
            if(start_value == 0) {
                buffer.position = start_position;
                SyntaxError error("zero-valued instance bound", buffer);
                errors.push_back(error);
                return nullptr;
            }
            instance_bounds = {start_value, start_value};
        }

        // N or more instances
        else if(start_value != -1 && end_value == -1 && colon_present)
            instance_bounds = {start_value, -1};

        // Up to N instances
        else if(start_value == -1 && end_value != -1 && colon_present) {
            if(end_value == 0) {
                buffer.position = start_position;
                SyntaxError error("up-to-zero instance bound", buffer);
                errors.push_back(error);
                return nullptr;
            }
            instance_bounds = {-1, end_value};
        }

        // Between N and M values
        else if(start_value != -1 && end_value != -1 && colon_present) {
            if(end_value < start_value) {
                buffer.position = start_position;
                SyntaxError error("invalid instance bound", buffer);
                errors.push_back(error);
                return nullptr;
            }
            else if(start_value == end_value && start_value == 0) {
                buffer.position = start_position;
                SyntaxError error("zero-instance bound", buffer);
                errors.push_back(error);
                return nullptr;
            }

            instance_bounds = {start_value, end_value};
        }

        else {
            buffer.position = start_position;
            SyntaxError error("malformed instance bounds", buffer);
            errors.push_back(error);
            return nullptr;
        }
    }
    else
        instance_bounds = {1, 1};

    term->predicate = predicate;
    term->silenced = silenced;
    term->instance_bounds = instance_bounds;
    return term;
}

std::shared_ptr<Term> Term::parse_constant(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::CONSTANT;

    // Check there's a constant
    if(buffer.read('\'') == false)
        throw ParseLogicError("no constant found", buffer);

    const TextBuffer::Position start_position = buffer.position;

    std::string value;
    while(true) {
        const char character = buffer.peek();

        if(buffer.read('\''))
            break;

        // Handle premature endings
        else if(buffer.end_reached()) {
            SyntaxError error("unexpected end-of-file in constant", buffer);
            errors.push_back(error);
            return nullptr;
        }
        else if(character == '\n' || character == '\r') {
            SyntaxError error("unexpected end-of-line in constant", buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Check values are ASCII characters
        else if(character < ' ' || character > '~') {
            SyntaxError error("non-ascii value in constant", buffer);
            errors.push_back(error);
            return nullptr;
        }

        else if(buffer.peek('\\')) {

            const TextBuffer::Position code_position = buffer.position;
            const char escape_code = parse_escape_code(buffer);
            if(escape_code == 0) {
                const SyntaxError error("invalid escape code in constant",
                        buffer);
                errors.push_back(error);
                return nullptr;
            }
            value += escape_code;
        }

        else
            value += buffer.read();
    }

    if(value.empty()) {
        buffer.position = start_position;
        const SyntaxError error("empty constant", buffer);
        errors.push_back(error);
        return nullptr;
    }

    term->value = value;
    return term;
}

std::shared_ptr<Term> Term::parse_range(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    // Create a term
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::RANGE;

    // Check there's a range present
    if(buffer.read('[') == false)
        throw ParseLogicError("no range found", buffer);

    // Parse start value
    buffer.skip_space();
    if(buffer.read('\'') == false) {
        const SyntaxError error("expected '\\\'' in range", buffer);
        errors.push_back(error);
        return nullptr;
    }

    char start_value;
    if(buffer.peek('\\')) {
        start_value = parse_escape_code(buffer);
        if(start_value < ' ' || start_value > '~') {
            const std::string message =
                    "non letter/symbol ASCII escape codes not permitted in "
                    "ranges";
            const SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }
    }
    else
        start_value = buffer.read();

    if(start_value < ' ' || start_value > '~') {
        const std::string message = "range start constants must be a valid "
                "ASCII letters, numbers, or symbols";
        SyntaxError error(message, buffer);
        errors.push_back(error);
        return nullptr;
    }

    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check for a hyphen between start and end values
    buffer.skip_space();
    if(buffer.read('-') == false) {
        SyntaxError error("expected '-'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Parse end value
    buffer.skip_space();
    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    char end_value;
    if(buffer.peek('\\')) {
        end_value = parse_escape_code(buffer);
        if(end_value < ' ' || end_value > '~') {
            const std::string message =
                    "non letter/symbol ASCII escape codes not permitted in "
                    "ranges";
            const SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }
    }
    else
        end_value = buffer.read();

    if(buffer.read('\'') == false) {
        SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check there's a closing bracket
    buffer.skip_space();
    if(buffer.read(']') == false) {
        SyntaxError error("expected ']'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check range makes logical sense
    bool errors_found = false;
    if(end_value < start_value) {
        SyntaxError error("range's end value less than start value", buffer);
        errors.push_back(error);
        errors_found = true;
    }

    // Check range's start and end values are ASCII
    else if(start_value < ' ' || start_value > '~' ) {
        SyntaxError error("range's start value is non-ASCII", buffer);
        errors.push_back(error);
        errors_found = true;
    }
    else if(end_value < ' ' || end_value > '~' ) {
        SyntaxError error("range's end value is non-ASCII", buffer);
        errors.push_back(error);
        errors_found = true;
    }

    if(errors_found)
        return nullptr;

    term->value = std::array<char, 2>({start_value, end_value});

    return term;
}

std::shared_ptr<Term> Term::parse_reference(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    // Create term
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::REFERENCE;

    // Extract characters
    std::string value;
    while(true) {
        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            value += buffer.read();
        else
            break;
    }

    // Check term found
    if(value.empty())
        throw ParseLogicError("no reference term found", buffer);

    // Check there aren't trailing invalid characters
    const char character = buffer.peek();
    static const std::unordered_set<char> terminators = {
        ' ', '\n', '\r', '\t', '#', // Whitespace
        '[', '\'',                  // Term
        '!', '&',                   // Predicates
        '(', '|'                    // Sequences
    };

    if(terminators.count(character) == 0) {
        const std::string message =
                "references can only contain lowercase letters and underscores";
        SyntaxError error(message, buffer);
        errors.push_back(error);
        return nullptr;
    }

    term->value = value;

    return term;
}

std::shared_ptr<Term> Term::parse_choice(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::CHOICE;
    term->instance_bounds = {1, 1};

    std::vector<std::shared_ptr<Term>> values;
    while(true) {
        const std::shared_ptr<Term> value = parse(buffer, errors);
        if(value == nullptr) {

            // Skip to the end of the sequence
            while(true) {
                buffer.skip_space();
                if(buffer.end_reached() || buffer.peek('\n'))
                    break;
                buffer.increment();
            }

            term = nullptr;
            break;
        }
        values.push_back(value);

        // Stop parsing terms if the end-of-file or end-of-line have been
        // reached, or if there's no pipe character
        if(buffer.end_reached()
                || buffer.peek('\n')
                || buffer.peek(')')
                || buffer.read('|') == false)
            break;

        // Check the file/line doesn't end given a pipe character's been found
        buffer.skip_space();
        if(buffer.peek('\n')) {
            const std::string message =
                    "unexpected end-of-line after choice operator";
            const SyntaxError error(message, buffer);
            errors.push_back(error);

            term = nullptr;
            break;
        }
        else if(buffer.peek(')')) {
            const SyntaxError error("unexpected ')' after choice operator",
                    buffer);
            errors.push_back(error);

            term = nullptr;
            break;
        }
        else if(buffer.end_reached()) {
            const std::string message =
                    "unexpected end-of-file after choice operator";
            const SyntaxError error(message, buffer);
            errors.push_back(error);

            term = nullptr;
            break;
        }
    }

    if(term) {
        if(values.size() == 1)
            term = values.back();
        else
            term->value = values;
    }

    return term;
}

std::shared_ptr<Term> Term::parse_sequence(TextBuffer &buffer,
        std::vector<SyntaxError> &errors) {

    // Create a term
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::SEQUENCE;

    // Parse elements
    std::vector<std::shared_ptr<Term>> values;
    while(true) {
        const std::shared_ptr<Term> value = parse_choice(buffer, errors);

        if(value == nullptr) {
            while(true) {
                buffer.skip_space();
                if(buffer.end_reached() || buffer.peek('\n'))
                    break;
                buffer.increment();
            }

            term = nullptr;
            break;
        }
        values.push_back(value);

        buffer.skip_space();
        if(buffer.end_reached() || buffer.peek('\n') || buffer.peek(')'))
            break;
    }

    if(term) {
        if(values.size() == 1)
            term = values.back();
        else
            term->value = values;
    }

    return term;
}

};
