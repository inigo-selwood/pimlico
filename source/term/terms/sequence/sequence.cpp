#include "sequence.hpp"

namespace Pimlico {

Sequence::Sequence() : Term(Term::Type::SEQUENCE) {}

Sequence::~Sequence() {
    while(this->values.empty() == false) {
        if(this->values.back())
            delete this->values.back();
        this->values.pop_back();
    }
}

}; // Namespace Pimlico
