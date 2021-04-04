#pragma once

#include "term.hpp"

class Rule {

public:

    Term value;

    std::string name;

    friend std::ostream &operator<<(std::ostream &stream, const Rule &rule) {
        return stream << rule.name << ": " << rule.value;
    }

    Rule(const std::string &name, const Term &value) {
        this->name = name;
        this->value = value;
    }

};
