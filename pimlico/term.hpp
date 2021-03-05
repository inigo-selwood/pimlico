#pragma once

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <variant>
#include <vector>

#include "text_buffer.hpp"

namespace Pimlico {

struct Error {

public:

    typedef std::vector<Error> Vector;

    Error(const std::string &message, const TextBuffer &buffer) {}

};

class Term {

public:

    // Used to deduce type for variant access
    enum class Type {
        CONSTANT,
        RANGE,

        REFERENCE,

        CHOICE,
        SEQUENCE,

        NONE,
    };

    // Used during tokenization to determine visibility
    enum class Condition {
        AND_PREDICATE,
        NOT_PREDICATE,

        SILENCED,

        SUPPRESSED,

        NONE,
    };

    typedef unsigned int Hash;
    typedef std::array<int, 2> Bounds;

    typedef std::shared_ptr<Term> SharedPointer;

    typedef std::string Constant;
    typedef std::array<char, 2> Range;

    typedef std::vector<SharedPointer> Choice;
    typedef std::vector<SharedPointer> Sequence;

    struct Reference {

    public:

        std::string name;

        SharedPointer pointer;

    };

    std::variant<Constant, Range, Reference, Choice, Sequence> value;

    // Used in error reporting
    TextBuffer::Position position;

    Bounds bounds;

    // In combination, a unique identifier
    std::string name;
    std::string scope;

    // Hash of the union of path and name
    unsigned int hash;

    // Number of scoped parents
    char scope_depth;

    Condition condition;

    Type type;

    Term(const Type &type,
            const std::string &scope,
            const unsigned int &scope_depth);
    Term(const Type &type,
            const std::string &name,
            const std::string &scope,
            const unsigned int &scope_depth);

    // Reference resolution helper
    static bool resolve_reference(Reference &reference,
            std::map<Hash, SharedPointer> &hash_terms);

    // Parse functions
    static SharedPointer parse_bytecode(TextBuffer &buffer);
    static SharedPointer parse_verbose(TextBuffer &buffer,
            Error::Vector &errors);

    // Print functions
    void print_bytecode(std::ostream &stream);
    void print_verbose(std::ostream &stream);

private:

    // Used for giving terms unique names
    static unsigned int count;

    // Bytecode descent parse functions
    static SharedPointer parse_bytecode_constant(TextBuffer &buffer);
    static SharedPointer parse_bytecode_range(TextBuffer &buffer);
    static SharedPointer parse_bytecode_reference(TextBuffer &buffer);
    static SharedPointer parse_bytecode_choice(TextBuffer &buffer);
    static SharedPointer parse_bytecode_sequence(TextBuffer &buffer);

    // Verbose descent parse functions
    static SharedPointer parse_verbose_constant(TextBuffer &buffer,
            Error::Vector &errors);
    static SharedPointer parse_verbose_range(TextBuffer &buffer,
            Error::Vector &errors);
    static SharedPointer parse_verbose_reference(TextBuffer &buffer,
            Error::Vector &errors);
    static SharedPointer parse_verbose_choice(TextBuffer &buffer,
            Error::Vector &errors);
    static SharedPointer parse_verbose_sequence(TextBuffer &buffer,
            Error::Vector &errors);

};

unsigned int Term::count = 0;

// ***************************************************************** Constructor

Term::Term(const Type &type,
        const std::string &scope,
        const unsigned int &scope_depth) : Term(type,
            "term_" + std::to_string(count),
            scope,
            scope_depth) {}

Term::Term(const Type &type,
        const std::string &name,
        const std::string &scope,
        const unsigned int &scope_depth) : bounds({1, 1}),
            name(name),
            scope(scope),
            scope_depth(scope_depth),
            condition(Condition::NONE),
            type(type) {

    hash = std::hash<std::string>{}(scope + "_" + name);
}

// ************************************************* Reference resolution helper

bool Term::resolve_reference(Term::Reference &reference,
        std::map<Term::Hash, Term::SharedPointer> &hash_terms) {


}

// ************************************************************* Print functions

void Term::print_bytecode(std::ostream &stream) {}

void Term::print_verbose(std::ostream &stream) {}

// ************************************************************* Parse functions

Term::SharedPointer Term::parse_bytecode(TextBuffer &buffer) {}

Term::SharedPointer Term::parse_verbose(TextBuffer &buffer,
        Error::Vector &errors) {}


// ******************************************** Bytecode descent parse functions

Term::SharedPointer Term::parse_bytecode_constant(TextBuffer &buffer) {}

Term::SharedPointer Term::parse_bytecode_range(TextBuffer &buffer) {}

Term::SharedPointer Term::parse_bytecode_reference(TextBuffer &buffer) {}

Term::SharedPointer Term::parse_bytecode_choice(TextBuffer &buffer) {}

Term::SharedPointer Term::parse_bytecode_sequence(TextBuffer &buffer) {}

// ********************************************* Verbose descent parse functions

Term::SharedPointer Term::parse_verbose_constant(TextBuffer &buffer,
        Error::Vector &errors) {

    /* Matches strings that:
    - Are enclosed by single quotes
    - Don't contain formatting characters
    - Can contain the escape codes: \' \" \\ \b \f \n \r \t \v

    Regex is the devil
    */
    static const std::regex
            regex("'((((?=[ -~])(?=[^\\\\]))|(\\\\[\'\"\\\\bfnrtv]))+)'");
    if(buffer.peek(regex).empty()) {
        errors.push_back(Error("syntax error in constant", buffer));
        return nullptr;
    }

}

Term::SharedPointer Term::parse_verbose_range(TextBuffer &buffer,
        Error::Vector &errors) {

    static const std::regex
            regex("\\[[ \t]*'[ -~]'[ \t]*-[ \t]*'[ -~]'[ \t]*\\]");
    if(buffer.peek(regex).empty()) {
        errors.push_back(Error("syntax error in range", buffer));
        return nullptr;
    }

    buffer.read('[');
    buffer.skip_whitespace();
    buffer.read('\'');
    const char start_value = buffer.read();
    buffer.read('\'');
    buffer.skip_whitespace();

    buffer.read('-');

    buffer.skip_whitespace();
    buffer.read('\'');
    const char end_value = buffer.read();
    buffer.read('\'');
    buffer.skip_whitespace();
    buffer.read(']');

    Range range({start_value, end_value});

    Term::SharedPointer term = Term::SharedPointer(Term::Type::RANGE,
            )
}

Term::SharedPointer Term::parse_verbose_reference(TextBuffer &buffer,
        Error::Vector &errors) {
    static const std::regex regex("[a-z0-9_]+");
    if(buffer.peek(regex).empty()) {
        errors.push_back(Error("syntax error in reference", buffer));
        return nullptr;
    }
}

Term::SharedPointer Term::parse_verbose_choice(TextBuffer &buffer,
        Error::Vector &errors) {

}

Term::SharedPointer Term::parse_verbose_sequence(TextBuffer &buffer,
        Error::Vector &errors) {

}

};
