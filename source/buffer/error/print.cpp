#include "error.hpp"

namespace Pimlico {

namespace Buffer {

/* Overloads the stream print operator for a term
Arguments
---------
stream
    the stream to print to
term
    the term to print
Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream, const Error &error) {
    error.print(stream);
    return stream;
}

/* Print a TextPosition instance
The printing format is:
`[<line>:<column>]`
Arguments
---------
stream
    the stream to print to
*/
void Error::print(std::ostream &stream) const {
    // int error_count = this->errors.size();
    // for(int index = 0; index < error_count; index += 1) {
    //     stream << this->errors[index];
    //     if((index + 1) < error_count)
    //         stream << '\n';
    // }

    stream << "error printing unimplemented\n";
}

}; // Namespace Buffer

}; // Namespace Pimlico
