#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Get the current character in the buffer

Returns
-------
character
    the buffer's character, or zero if the end of the buffer has been reached
*/
char Parse::get_character() const {
    if(this->position.index == this->length)
        return 0;

    return this->text[this->position.index];
}

}; // Namespace Buffer

}; // Namespace Pimlico
