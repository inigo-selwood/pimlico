#pragma once

#include <pimlico.hpp>

namespace Pimlico {

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

    Buffer::Position position;

    Bounds bounds;

    Type type;

    Predicate predicate;

    friend std::ostream &operator<<(std::ostream &stream, const Term &term);

    Term(const Term::Type &type);

    static Term *parse(Buffer::Parse &buffer,
            Buffer::Error &errors,
            const bool root = false);

    virtual void print(std::ostream &stream) const = 0;

    virtual ~Term() = default;

};

}; // Namespace Pimlico
