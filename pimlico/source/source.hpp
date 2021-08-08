#pragma once

#include <map>

#include "../rule/rule.hpp"

class Source {

public:

    std::map<int, Rule *> rules;

    static Source *parse(const std::string &grammar, ErrorBuffer &errors);

    ~Source();

};
