#include "production.hpp"

namespace Pimlico {

// Constructor
Production::Production() {
    this->value = nullptr;
}

// Destructor
Production::~Production() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
