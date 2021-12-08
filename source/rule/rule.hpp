#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Term;

class Rule {

public:

    Buffer::Position position;

    Term *value;

    std::string name;

    Rule();

    static Rule *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    ~Rule();

};

}; // Namespace Pimlico
