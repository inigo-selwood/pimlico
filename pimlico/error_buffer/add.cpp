#include "error_buffer.hpp"

#include "error/errors/message/message.hpp"
#include "error/errors/reference/reference.hpp"

/* Add an error to the buffer

Arguments
---------
message
    the message associated with the error
buffer
    the buffer to infer current position and text from
*/
void ErrorBuffer::add(const std::string &message, const ParseBuffer &buffer) {
    this->errors.push_back(new Reference(message, buffer));
}

/* Add an error to the buffer

Arguments
---------
message
    the message associated with the error
buffer
    the buffer to infer current position and text from
position
    the position within the buffer to report an error at
*/
void ErrorBuffer::add(const std::string &message,
        const ParseBuffer &buffer,
        const TextPosition &position) {

    this->errors.push_back(new Reference(message, buffer, position));
}

void ErrorBuffer::add(const std::string &message) {
    this->errors.push_back(new Message(message));
}
