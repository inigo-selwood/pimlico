#include "parse_buffer.hpp"

/* Look at the current character in the buffer, without incrementing

Returns
-------
character
    the current character, or zero if the end of the buffer has been reached
*/
char ParseBuffer::peek() const {
    return get_character();
}

/* Check if a character is present at the buffer's position

Arguments
---------
character
    the character to check for

Returns
-------
present
    if the character is present at the buffer's current position
*/
bool ParseBuffer::peek(const char &character) const {
    return get_character() == character;
}

/* Check if a string is present at the buffer's position

Arguments
---------
string
    the string to check for

Returns
-------
present
    if the string is present at the buffer's current position
*/
bool ParseBuffer::peek(const std::string &string) const {
    if(this->position.index + string.length() >= this->length)
        return false;

    return this->text.substr(this->position.index, string.length()) == string;
}
