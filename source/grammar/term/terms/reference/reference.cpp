#include "reference.hpp"

namespace Pimlico {

// Constructor
Reference::Reference() : Term(Term::Type::REFERENCE) {
    this->value = nullptr;
}

// Destructor
Reference::~Reference() {

    // Free memory held by the value field
    if(this->value) {
        delete this->value;
        this->value = nullptr;
    }
}

}; // Namespace Pimlico
