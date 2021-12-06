#include "choice.hpp"

namespace Pimlico {

Choice::Choice() : Term(Term::Type::SEQUENCE) {}

Choice::~Choice() {
    while(this->values.empty() == false) {
        if(this->values.back())
            delete this->values.back();

        this->values.pop_back();
    }
}


}; // Namespace Pimlico
