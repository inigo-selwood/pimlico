#include "error_buffer.hpp"

/* Output stream operator overload for the buffer

Arguments
---------
stream
    the stream to print to
errors
    the error buffer whose errors to print

Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream, const ErrorBuffer &errors) {
    errors.print(stream);
    return stream;
}

/* Prints the contents of an error buffer

Arguments
---------
stream
    the stream to print to
*/
void ErrorBuffer::print(std::ostream &stream) const {
    int error_count = this->errors.size();
    for(int index = 0; index < error_count; index += 1) {
        stream << this->errors[index];
        if((index + 1) < error_count)
            stream << '\n';
    }
}
