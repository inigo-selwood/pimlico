#include "error_buffer.hpp"

/* Add an error to the buffer 

Arguments 
---------
message
    the message associated with the error
position
    the position of the error within the buffer
*/
void ErrorBuffer::add(const std::string &message, 
        const TextPosition &position) {
    
    this->errors.push_back({message, position});
}