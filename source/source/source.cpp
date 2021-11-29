#include "source.hpp"

Source::~Source() {
    for(auto iterator = rules.begin(); iterator != rules.end(); iterator ++) {
        if(iterator->second) {
            delete iterator->second;
            iterator->second = nullptr;
        }
    }
}
