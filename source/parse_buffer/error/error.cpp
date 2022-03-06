#include "../parse_buffer.hpp"

namespace Pimlico {

ParseBuffer::Error::~Error() {
    while(this->instances.empty() == false) {
        if(this->instances.back())
            delete this->instances.back();
        this->instances.pop_back();
    }
}

}; // Namespace Pimlico
