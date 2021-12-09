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
    int instance_count = this->instances.size();
    for(int index = 0; index < instance_count; index += 1) {
        this->instances[index]->print(stream);
        if((index + 1) < instance_count)
            stream << '\n';
    }
}

}; // Namespace Buffer

}; // Namespace Pimlico
