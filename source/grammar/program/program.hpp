#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Rule;

class Program {

public:

    std::map<int, Rule *> rules;

    friend std::ostream &operator<<(std::ostream &stream,
            const Program &program);

    static Program *parse(const std::string &grammar, Buffer::Error &errors);

    ~Program();

};

}; // Namespace Pimlico
