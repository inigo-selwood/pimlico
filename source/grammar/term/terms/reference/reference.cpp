#include "reference.hpp"

namespace Pimlico {

Reference::Reference() : Term(Term::Type::REFERENCE) {}

Reference::~Reference() {
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
