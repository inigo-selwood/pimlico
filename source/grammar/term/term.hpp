#pragma once

#include <map>

#include <pimlico.hpp>

namespace Pimlico {

class Rule;

class Term {

public:

    typedef std::array<int, 2> Bounds;

    enum class Type {
        NONE,

        CONSTANT,
        RANGE,
        REFERENCE,

        CHOICE,
        SEQUENCE,
    };

    enum class Predicate {
        NONE,

        AND,
        NOT,
    };

    ParseBuffer::Position position;

    std::string binding;

    Bounds bounds;

    Type type;

    Predicate predicate;

    friend std::ostream &operator<<(std::ostream &stream, const Term &term);

    Term(const Term::Type &type);

    static Term *parse(ParseBuffer &buffer,
            ParseBuffer::Error &errors,
            const bool root = false);

    virtual void print(std::ostream &stream) const = 0;

    virtual bool emplaceRules(std::map<std::string, Rule *> &rules,
            ParseBuffer::Error &errors);

    virtual ~Term() = default;

};

}; // Namespace Pimlico
