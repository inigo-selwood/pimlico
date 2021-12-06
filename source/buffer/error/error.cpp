#include "error.hpp"

namespace Pimlico {

namespace Buffer {

Error::~Error() {
    while(this->errors.empty() == false) {
        Error *error = this->errors.back();
        if(error)
            delete error;
        this->errors.pop_back();
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
