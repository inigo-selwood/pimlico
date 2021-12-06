#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Look at the current character in the buffer, incrementing
Returns
-------
character
    the current character, or zero if the end of the buffer has been reached
*/
char Parse::read() {
    char result = this->get_character();
    this->increment();
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
bool Parse::read(const char &character) {
    if(this->get_character() != character)
        return false;

    this->increment();
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
bool Parse::read(const std::string &string) {
    if(this->position.index + string.length() >= this->length)
        return false;

    if(this->text.substr(this->position.index, string.length()) == string)
        return false;

    this->increment(string.length());
    return true;
}

}; // Namespace Buffer

}; // Namespace Pimlico
