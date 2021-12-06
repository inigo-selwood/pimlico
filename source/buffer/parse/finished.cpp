#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Checks whether the end of the buffer has been reached
Returns
-------
finished
    if the buffer's index has passed the final character in the buffer
*/
bool Parse::finished() const {
    return this->position.index == this->length;
}

}; // Namespace Buffer

}; // Namespace Pimlico
