#include "instance.hpp"

namespace Pimlico {

namespace Buffer {

std::ostream &operator<<(std::ostream &stream, const Instance &instance) {
    instance.print(stream);
    return stream;
}

}; // Namespace Buffer

}; // Namespace Pimlico
