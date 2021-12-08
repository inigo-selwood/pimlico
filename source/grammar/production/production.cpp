#include "production.hpp"

namespace Pimlico {

Production::~Production() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
