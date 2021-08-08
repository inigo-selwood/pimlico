#include "error_buffer.hpp"

/* Output stream operator overload for the buffer

Arguments
---------
stream
    the stream to print to
buffer
    the buffer whose errors to print

Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream, const ErrorBuffer &buffer) {
    buffer.print(stream);
    return stream;
}

/* Prints the contents of an error buffer 

Arguments
---------
stream
    the stream to print to
*/
void ErrorBuffer::print(std::ostream &stream) const {
    // stream << "error count: " << this->errors.size() << '\n';
    for(const auto &error : this->errors)
        stream << error.first << ' ' << error.second << '\n';
}