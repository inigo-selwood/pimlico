#include "program.hpp"

namespace Pimlico {

Program::~Program() {
    for(auto iterator = rules.begin(); iterator != rules.end(); iterator ++) {
        if(iterator->second) {
            delete iterator->second;
            iterator->second = nullptr;
        }
    }
}

}; // Namespace Pimlico
