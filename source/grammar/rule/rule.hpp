#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Production;

class Rule {

public:

    Buffer::Position position;

    std::vector<Production *> productions;

    std::string name;
    std::string type;

    Rule();

    static Rule *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    ~Rule();

};

}; // Namespace Pimlico
