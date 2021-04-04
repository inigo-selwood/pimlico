#pragma once

#include "rule.hpp"

class Root {

public:

    std::vector<Rule> rules;

    friend std::ostream &operator<<(std::ostream &stream, const Root &root) {
        for(const auto &rule : root.rules)
            stream << rule << '\n';
        return stream;
    }

    Root(const std::vector<Rule> &rules) {
        this->rules = rules;
    }

};
