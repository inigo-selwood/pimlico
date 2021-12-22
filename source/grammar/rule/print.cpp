#include "rule.hpp"

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
std::ostream &operator<<(std::ostream &stream, const Rule &rule) {
    stream << rule.name << ' ';
    if(rule.type != "")
        stream << '<' << rule.type << "> ";

    stream << ":=\n";
    for(const Production *production : rule.productions)
        stream << "    " << *production << '\n';

    return stream;
}

}; // Namespace Pimlico
