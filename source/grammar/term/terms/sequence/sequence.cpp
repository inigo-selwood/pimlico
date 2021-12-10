#include "sequence.hpp"

namespace Pimlico {

// Constructor
Sequence::Sequence() : Term(Term::Type::SEQUENCE) {}

// Destructor
Sequence::~Sequence() {

    // Delete entries in the values vector
    while(this->values.empty() == false) {
        if(this->values.back())
            delete this->values.back();
        this->values.pop_back();
    }
}

}; // Namespace Pimlico
