#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Term;

class Production {

public:

    Buffer::Position position;

    Term *value;

    std::string expression;

    friend std::ostream &operator<<(std::ostream &stream,
            const Production &production);

    Production();

    static Production *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    ~Production();

};

}; // Namespace Pimlico
