#include "parse_buffer.hpp"

namespace Pimlico {

/* Checks whether the end of the buffer has been reached
Returns
-------
finished
    if the buffer's index has passed the final character in the buffer
*/
bool ParseBuffer::finished() const {
    return this->position.index == this->length;
}

}; // Namespace Pimlico
