#pragma once

#include <array>
#include <string>
#include <vector>

#include "../parse_buffer/parse_buffer.hpp"
#include "../error_buffer/error_buffer.hpp"
#include "../text_position/text_position.hpp"

class Term {

public:

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

    TextPosition position;

    typedef std::array<int, 2> Bounds;

    Bounds bounds;

    Type type;

    Predicate predicate;

    friend std::ostream &operator<<(std::ostream &stream, const Term &term);

    Term(const Term::Type &type);

    static Term *parse(ParseBuffer &buffer,
            ErrorBuffer &errors,
            const bool root);

    virtual void print(std::ostream &stream) const = 0;

    virtual ~Term() = default;

};
