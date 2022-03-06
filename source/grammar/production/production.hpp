#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Term;

class Production {

public:

    ParseBuffer::Position position;

    Term *value;

    std::string expression;

    friend std::ostream &operator<<(std::ostream &stream,
            const Production &production);

    Production();

    static Production *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    ~Production();

};

}; // Namespace Pimlico
