#include "production.hpp"

namespace Pimlico {

Production::Production() {
    this->value = nullptr;
}

Production::~Production() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
