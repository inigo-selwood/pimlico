#include "program.hpp"

namespace Pimlico {

/* Overloads the stream print operator for a program
Arguments
---------
stream
    the stream to print to
program
    the program to print

Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const Program &program) {

    for(const std::pair<int, Rule *> &pair : program.rules) {
        stream << *(pair.second) << '\n';
    }

    return stream;
}

}; // Namespace Pimlico
