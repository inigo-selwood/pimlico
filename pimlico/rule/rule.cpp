#include "rule.hpp"

Rule::Rule() {
    this->value = nullptr;
}

Rule::~Rule() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}
