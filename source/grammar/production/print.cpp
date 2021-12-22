#include "production.hpp"

namespace Pimlico {

/* Overloads the stream print operator for a rule
Arguments
---------
stream
    the stream to print to
rule
    the rule to print
Returns
-------
stream
    the stream, having been printed to
*/
std::ostream &operator<<(std::ostream &stream,
        const Production &production) {

    if(production.value == nullptr)
        stream << "null ";
    else
        stream << *(production.value) << ' ';

    if(production.expression != "")
        stream << '{' << production.expression << '}';

    return stream;
}

}; // Namespace Pimlico
