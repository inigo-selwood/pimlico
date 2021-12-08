#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Rule;

class Program {

public:

    std::map<int, Rule *> rules;

    static Program *parse(const std::string &grammar, Buffer::Error &errors);

    ~Program();

};

}; // Namespace Pimlico
