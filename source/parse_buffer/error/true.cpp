#include "../parse_buffer.hpp"

namespace Pimlico {

ParseBuffer::Error::operator bool() const {
    return this->instances.empty() == false;
}

}; // Namespace Pimlico
