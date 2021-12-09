#include "choice.hpp"

namespace Pimlico {

// Constructor
Choice::Choice() : Term(Term::Type::SEQUENCE) {}

// Destructor
Choice::~Choice() {

    // Delete any entries in the values vector
    while(this->values.empty() == false) {
        if(this->values.back())
            delete this->values.back();

        this->values.pop_back();
    }
}


}; // Namespace Pimlico
