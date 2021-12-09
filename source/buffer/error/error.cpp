#include "error.hpp"

namespace Pimlico {

namespace Buffer {

Error::~Error() {
    while(this->instances.empty() == false) {
        if(this->instances.back())
            delete this->instances.back();
        this->instances.pop_back();
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
