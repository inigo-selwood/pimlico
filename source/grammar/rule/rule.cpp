#include "rule.hpp"

namespace Pimlico {

// Destructor
Rule::~Rule() {
    while(this->productions.empty() == false) {
        if(this->productions.back())
            delete this->productions.back();
        this->productions.pop_back();
    }
}

}; // Namespace Pimlico
