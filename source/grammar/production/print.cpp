#include "production.hpp"

namespace Pimlico {

/* Overloads the stream print operator for a production
Arguments
---------
stream
    the stream to print to
rule
    the production to print

Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const Production &production) {

    // Note: Can't use a neater ternary operator here, since type-mismatch
    if(production.value == nullptr)
        stream << "null";
    else
        stream << *(production.value);

    if(production.expression != "")
        stream << " {" << production.expression << '}';

    return stream;
}

}; // Namespace Pimlico
