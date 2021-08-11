#include "parse_buffer.hpp"

/* Checks whether the end of the buffer has been reached

Returns
-------
finished
    if the buffer's index has passed the final character in the buffer
*/
bool ParseBuffer::finished() const {
    return (this->position.index + 1) == this->length;
}
