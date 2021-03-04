#pragma once

#define DEBUG

#include <array>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "parse_logic_error.hpp"
#include "text_buffer.hpp"

namespace Pimlico {

class Term {

public:

    struct Reference {

    public:

        std::string name;

        std::shared_ptr<Term> value;

        Reference(const std::string &name) {
            this->name = name;
        }

    };

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

    std::variant<std::string,
            Reference,
            std::array<char, 2>,
            std::vector<std::shared_ptr<Term>>> value;

    TextBuffer::Position position;

    std::array<int, 2> instance_bounds;

    Type type;

    Predicate predicate;

    bool silenced;

    friend std::ostream &operator<<(std::ostream &stream, const Term &term);

    Term() : instance_bounds({1, 1}),
            type(Type::NONE),
            predicate(Predicate::NONE),
            silenced(false) {}

    static std::shared_ptr<Term> parse(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors,
            const bool root);

private:

    static std::shared_ptr<Term> parse_constant(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);
    static std::shared_ptr<Term> parse_range(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);
    static std::shared_ptr<Term> parse_reference(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);

    static std::shared_ptr<Term> parse_choice(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);
    static std::shared_ptr<Term> parse_sequence(TextBuffer &buffer,
            std::vector<TextBuffer::SyntaxError> &errors);

};

/* Pretty-print a term

Arguments:
    stream (std::ostream &): the stream to print to
    term (const Term &): the term to print
*/
std::ostream &operator<<(std::ostream &stream, const Term &term) {

    // Serialize predicates
    if(term.predicate == Term::Predicate::AND)
        stream << "&";
    else if(term.predicate == Term::Predicate::NOT)
        stream << "!";

    // Serialize silenced symbol
    else if(term.silenced)
        stream << "$";

    static std::map<char, std::string> escape_codes = {
        {'\n', "\\n"},
        {'\r', "\\r"} ,
        {'\b', "\\b"},
        {'\t', "\\t"},
        {'\\', "\\\\"},
        {'\"', "\\\""},
        {'\'', "\\\'"},
    };

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
        const std::array<char, 2> &range =
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
        stream << std::get<Term::Reference>(term.value).name;

    // Serialize choices and sequences
    else if(type == Term::Type::CHOICE || type == Term::Type::SEQUENCE) {
        const std::vector<std::shared_ptr<Term>> &values =
                std::get<std::vector<std::shared_ptr<Term>>>(term.value);

        bool enclosed = false;
        if(term.instance_bounds != std::array<int, 2>({1, 1})) {
            enclosed = true;
            stream << "(";
        }

        for(unsigned int index = 0; index < values.size(); index += 1) {
            const std::shared_ptr<Term> &pointer = values[index];

            // Enclose the term, enclosing it if necessary
            bool child_enclosed = false;
            if(type == Term::Type::CHOICE
                        && pointer->type == Term::Type::SEQUENCE) {
                stream << "(";
                child_enclosed = true;
            }
            stream << *pointer;
            if(child_enclosed)
                stream << ")";

            // Print a seperator
            if(index + 1 < values.size()) {
                if(type == Term::Type::CHOICE)
                    stream << " | ";
                else if(type == Term::Type::SEQUENCE)
                    stream << " ";
            }
        }

        if(enclosed)
            stream << ")";
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

/* Parse a positive integer

Arguments:
    buffer (TextBuffer &): the buffer containing an integer at its current index
*/
static int parse_integer(TextBuffer &buffer) {

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
    if(text.empty())
        throw ParseLogicError("parse_integer called with no integer", buffer);

    // Parse and return integer
    try {
        return std::stoi(text);
    }
    catch(...) {
        return -1;
    }
}

static inline std::array<int, 2> parse_instance_bounds(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    std::array<int, 2> instance_bounds;
    if(buffer.read('?'))
        return {0, 1};
    else if(buffer.read('*'))
        return {0, -1};
    else if(buffer.read('+'))
        return {1, -1};

    // Parse instance ranges
    else if(buffer.read('{') == false)
        return {1, 1};

    // Parse bound start value (if present)
    buffer.skip_space();
    int start_value = -1;
    char character = buffer.peek();
    if(character >= '0' && character <= '9') {
        start_value = parse_integer(buffer);
        if(start_value == -1) {
            const std::string message =
                    "invalid instance bound start value";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return {0, 0};
        }
    }

    // Check to see if a colon is present
    buffer.skip_space();
    bool colon_present = false;
    if(buffer.read(':'))
        colon_present = true;

    // Parse bound end value (if present)
    buffer.skip_space();
    int end_value = -1;
    character = buffer.peek();
    if(character >= '0' && character <= '9') {
        end_value = parse_integer(buffer);
        if(end_value == -1) {
            const std::string message =
                    "invalid instance bound end value";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return {0, 0};
        }
    }

    // Check for a closing bracket
    buffer.skip_space();
    if(buffer.read('}') == false) {
        const std::string message =
                "expected '}' at end of instance bound";
        const TextBuffer::SyntaxError error(message, buffer);
        errors.push_back(error);
        return {0, 0};
    }

    // N instances
    if(start_value != -1 && end_value == -1 && colon_present == false) {
        if(start_value == 0) {
            const TextBuffer::SyntaxError error("zero-valued instance bound",
                    buffer,
                    &position);
            errors.push_back(error);
            return {0, 0};
        }
        return {start_value, start_value};
    }

    // N or more instances
    else if(start_value != -1 && end_value == -1 && colon_present)
        return {start_value, -1};

    // Up to N instances
    else if(start_value == -1 && end_value != -1 && colon_present) {
        if(end_value == 0) {
            const TextBuffer::SyntaxError error("up-to-zero instance bound",
                    buffer,
                    &position);
            errors.push_back(error);
            return {0, 0};
        }
        return {-1, end_value};
    }

    // Between N and M values
    else if(start_value != -1 && end_value != -1 && colon_present) {
        if(end_value < start_value) {
            const TextBuffer::SyntaxError error("invalid instance bound",
                    buffer,
                    &position);
            errors.push_back(error);
            return {0, 0};
        }
        else if(start_value == end_value && start_value == 0) {
            const TextBuffer::SyntaxError error("zero-instance bound",
                    buffer,
                    &position);
            errors.push_back(error);
            return {0, 0};
        }

        return {start_value, end_value};
    }

    // Report an error if the bound was invalid
    else {
        const TextBuffer::SyntaxError error("malformed instance bounds",
                buffer,
                &position);
        errors.push_back(error);
        return {0, 0};
    }

    return {1, 1};
}

std::shared_ptr<Term> Term::parse(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors,
        const bool root = false) {

    if(root)
        return parse_sequence(buffer, errors);

    const TextBuffer::Position position = buffer.position;

    // Parse predicate
    Predicate predicate = Predicate::NONE;
    if(buffer.read('&'))
        predicate = Predicate::AND;
    else if(buffer.read('!'))
        predicate = Predicate::NOT;

    // Parse silencing hint
    buffer.skip_space();
    bool silenced = false;
    if(buffer.read('$')) {

        buffer.skip_space();
        if(buffer.peek('&')
                || buffer.peek('|')
                || predicate != Predicate::NONE) {
            const std::string message =
                    "unneccessarily silenced and predicated term";
            const TextBuffer::SyntaxError error(message, buffer, &position);
            errors.push_back(error);
            return nullptr;
        }

        silenced = true;
    }

    // Create a term instance
    std::shared_ptr<Term> term = nullptr;

    // Check if term should be parsed as a sequence since it's a root, or
    // enclosed
    buffer.skip_space();
    bool enclosed = buffer.read('(');

    if(enclosed)
        term = parse_sequence(buffer, errors);

    // Otherwise, look for a constant, range, or reference
    else {
        const char character = buffer.peek();

        if(character == '\'')
            term = parse_constant(buffer, errors);
        else if(character == '[')
            term = parse_range(buffer, errors);
        else if((character >= 'a' && character <= 'z') || character == '_')
            term = parse_reference(buffer, errors);
        else {
            const TextBuffer::SyntaxError error("expected a term", buffer);
            errors.push_back(error);
            return nullptr;
        }
    }

    // Check the term was parsed properly
    if(term == nullptr)
        return nullptr;

    // Check for a closing bracket if the term was enclosed
    buffer.skip_space();
    if(enclosed && buffer.read(')') == false) {
        const TextBuffer::SyntaxError error("expected ')'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Parse instance hints
    buffer.skip_space();
    const std::array<int, 2> instance_bounds =
            parse_instance_bounds(buffer, errors);
    if(instance_bounds == std::array<int, 2>({0, 0}))
        return nullptr;

    term->silenced = silenced;
    term->predicate = predicate;
    term->silenced = silenced;
    term->instance_bounds = instance_bounds;

    return term;
}

static char parse_escape_code(TextBuffer &buffer) {

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

std::shared_ptr<Term> Term::parse_constant(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    // Check there's a constant
    if(buffer.read('\'') == false)
        throw ParseLogicError("no constant found", buffer);

    // Parse the constant
    std::string value;
    while(true) {
        const char character = buffer.peek();
        if(buffer.read('\''))
            break;

        // Handle invalid characters
        else if(character == '\t' || character == '\r') {
            const std::string message = "invalid character in constant";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Handle premature endings
        else if(buffer.end_reached()) {
            const std::string message = "unexpected end-of-file in constant";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }
        else if(character == '\n' || character == '\r') {
            const std::string message = "unexpected end-of-line in constant";
            const TextBuffer::SyntaxError error(message, buffer);
            errors.push_back(error);
            return nullptr;
        }

        // Handle escape codes
        else if(buffer.peek('\\')) {
            const char escape_code = parse_escape_code(buffer);
            if(escape_code == 0) {
                const std::string message =
                        "invalid escape character in constant";
                const TextBuffer::SyntaxError error(message, buffer);
                errors.push_back(error);
                return nullptr;
            }

            value += escape_code;
        }

        else
            value += buffer.read();
    }

    // Check the constant wasn't empty
    if(value.empty()) {
        const TextBuffer::SyntaxError error("empty constant",
                buffer,
                &position);
        errors.push_back(error);
        return nullptr;
    }

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::CONSTANT;
    term->position = position;
    term->value = value;
    return term;
}

std::shared_ptr<Term> Term::parse_range(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    // Check for opening bracket
    if(buffer.read('[') == false)
        throw ParseLogicError("no range found", buffer);

    // Parse start value
    buffer.skip_space();
    if(buffer.read('\'') == false) {
        TextBuffer::SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Handle escape codes
    char start_value = 0;
    if(buffer.peek('\\')) {
        const char code = parse_escape_code(buffer);
        if(code == -1 || code < ' ' || code > '~') {
            TextBuffer::SyntaxError error("invalid escape character", buffer);
            errors.push_back(error);
            return nullptr;
        }

        start_value = code;
    }
    else
        start_value = buffer.read();

    if(buffer.read('\'') == false) {
        TextBuffer::SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check for hyphen
    buffer.skip_space();
    if(buffer.read('-') == false) {
        TextBuffer::SyntaxError error("expected '-'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Parse end value
    buffer.skip_space();
    if(buffer.read('\'') == false) {
        TextBuffer::SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Handle escape codes
    char end_value = 0;
    if(buffer.peek('\\')) {
        const char code = parse_escape_code(buffer);
        if(code == -1 || code < ' ' || code > '~') {
            TextBuffer::SyntaxError error("invalid escape character", buffer);
            errors.push_back(error);
            return nullptr;
        }

        end_value = code;
    }
    else
        end_value = buffer.read();

    if(buffer.read('\'') == false) {
        TextBuffer::SyntaxError error("expected '\\\''", buffer);
        errors.push_back(error);
        return nullptr;
    }

    // Check for end bracket
    buffer.skip_space();
    if(buffer.read(']') == false) {
        TextBuffer::SyntaxError error("expected ']'", buffer);
        errors.push_back(error);
        return nullptr;
    }

    if(start_value >= end_value) {
        TextBuffer::SyntaxError error("illogical range values",
                buffer,
                &position);
        errors.push_back(error);
        return nullptr;
    }

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::RANGE;
    term->position = position;
    term->value = std::array<char, 2>({start_value, end_value});

    return term;
}

std::shared_ptr<Term> Term::parse_reference(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    // Extract characters
    std::string name;
    while(true) {
        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') || character == '_')
            name += buffer.read();
        else
            break;
    }

    // Check a term was found
    if(name.empty())
        throw ParseLogicError("no reference found", buffer);

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::REFERENCE;
    term->position = position;
    term->value = Term::Reference(name);

    return term;
}

std::shared_ptr<Term> Term::parse_choice(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    // Parse options
    std::vector<std::shared_ptr<Term>> values;
    while(true) {

        // Parse option
        const std::shared_ptr<Term> value = parse(buffer, errors);
        if(value == nullptr)
            return nullptr;
        values.push_back(value);

        // Stop parsing terms if the end-of-file or end-of-line have been
        // reached, or if there's no pipe character
        buffer.skip_space(true);
        if(buffer.read('|') == false)
            break;

        // Report an error if an invalid state is encountered after the choice
        // operator has already been encountered
        enum class ErrorState {
            NONE,

            END_OF_FILE,
            END_OF_LINE,
            END_OF_STATEMENT,
        };
        ErrorState state = ErrorState::NONE;

        static std::map<ErrorState, std::string> error_messages = {
            {ErrorState::END_OF_FILE,
                    "unexpected end-of-file after choice operator"},
            {ErrorState::END_OF_LINE,
                    "unexpected end-of-line after choice operator"},
            {ErrorState::END_OF_STATEMENT,
                    "unexpected ')' after choice operator"}
        };

        buffer.skip_space(true);
        if(buffer.end_reached())
            state = ErrorState::END_OF_FILE;
        else if(buffer.peek('\n'))
            state = ErrorState::END_OF_LINE;
        else if(buffer.peek(')'))
            state = ErrorState::END_OF_STATEMENT;

        if(state != ErrorState::NONE) {
            const TextBuffer::SyntaxError error(error_messages[state], buffer);
            errors.push_back(error);
            return nullptr;
        }
    }

    // If the choice only has one option, return that instead
    if(values.size() == 1)
        return values.back();

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::CHOICE;
    term->position = position;
    term->value = values;

    return term;
}

std::shared_ptr<Term> Term::parse_sequence(TextBuffer &buffer,
        std::vector<TextBuffer::SyntaxError> &errors) {

    const TextBuffer::Position position = buffer.position;

    // Parse elements
    std::vector<std::shared_ptr<Term>> values;
    while(true) {

        // Parse value
        const std::shared_ptr<Term> value = parse_choice(buffer, errors);
        if(value == nullptr)
            return nullptr;
        values.push_back(value);

        buffer.skip_space(true);
        if(buffer.end_reached() || buffer.peek('\n') || buffer.peek(')'))
            break;
    }

    // If the sequence only has one term, return that instead
    if(values.size() == 1)
        return values.back();

    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    term->type = Term::Type::SEQUENCE;
    term->position = position;
    term->value = values;

    return term;
}

};
