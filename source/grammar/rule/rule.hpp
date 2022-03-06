#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Production;

class Rule {

public:

    ParseBuffer::Position position;

    std::vector<Production *> productions;

    std::string name;
    std::string type;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule);

    Rule();

    static Rule *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    ~Rule();

};

}; // Namespace Pimlico
