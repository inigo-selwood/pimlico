#include "error.hpp"

namespace Pimlico {

namespace Buffer {

Error::operator bool() const {
    return this->instances.empty() == false;
}

}; // Namespace Buffer

}; // Namespace Pimlico
