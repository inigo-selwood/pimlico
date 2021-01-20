#pragma once

#include <string>
#include <variant>
#include <vector>

namespace Pimlico {

class Token {

public:

    std::variant<std::string, std::vector<Token>> value;

    std::string name;

    unsigned int hash;

    bool terminal;

};


};
