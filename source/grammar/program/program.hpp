#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Rule;

class Program {

public:

    std::map<std::string, Rule *> rules;

    friend std::ostream &operator<<(std::ostream &stream,
            const Program &program);

    static Program *parse(const std::string &grammar,
            ParseBuffer::Error &errors);

    ~Program();

};

}; // Namespace Pimlico
