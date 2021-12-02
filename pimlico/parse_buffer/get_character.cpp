#include "parse_buffer.hpp"

/* Get the current character in the buffer

Returns
-------
character
    the buffer's character, or zero if the end of the buffer has been reached
*/
char ParseBuffer::get_character() const {
    if(this->position.index == this->length)
        return 0;

    return this->text[this->position.index];
}
