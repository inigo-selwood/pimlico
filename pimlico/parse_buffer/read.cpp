#include "parse_buffer.hpp"

/* Look at the current character in the buffer, incrementing

Returns
-------
character
    the current character, or zero if the end of the buffer has been reached
*/
char ParseBuffer::read() {
    char result = get_character();
    increment();
    return result;
}

/* Check if a character is present at the buffer's position, incrementing if
found

Arguments
---------
character
    the character to check for

Returns
-------
present
    if the character is present at the buffer's current position
*/
bool ParseBuffer::read(const char &character) {
    if(get_character() != character)
        return false;

    increment();
    return true;
}

/* Check if a string is present at the buffer's position, incrementing if found

Arguments
---------
string
    the string to check for

Returns
-------
present
    if the string is present at the buffer's current position
*/
bool ParseBuffer::read(const std::string &string) {
    if(this->position.index + string.length() >= this->length)
        return false;

    if(this->text.substr(this->position.index, string.length()) == string)
        return false;

    increment(string.length());
    return true;
}
