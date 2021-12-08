#include "rule.hpp"

namespace Pimlico {

Rule::Rule() {
    this->value = nullptr;
}

Rule::~Rule() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
