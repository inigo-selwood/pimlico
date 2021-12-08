#include "term.hpp"

namespace Pimlico {

/* Print term bounds
Prints instance bounds, where the following formats are used:
```
# Single-character hints
term+   # One-or-more instances
term*   # Zero-or-more instances
term?   # An optional instance
# Specific intance bounds
term{n}     # N instances
term{:n}    # Up to N instances
term{n:}    # N or more instances
term{n:m}   # Between N and M instances
```
Arguments
---------
stream
    the stream to print the instance bounds to
bounds
    the bounds to print
*/
static void print_bounds(std::ostream &stream, const Term::Bounds &bounds) {
    const int &lower = bounds[0];
    const int &upper = bounds[1];

    // Don't print anything more if the bounds are default
    if(lower == upper && lower == 1)
        return;

    // Serialize instance hints
    else if(lower == 0 && upper == 1)
        stream << "?";
    else if(lower == 0 && upper == -1)
        stream << "*";
    else if(lower == 1 && upper == -1)
        stream << "+";

    // Serialize instance bounds
    else if(lower == upper)
        stream << "{" << lower << "}";
    else if(lower == -1)
        stream << "{:" << upper << "}";
    else if(upper == -1)
        stream << "{" << lower << ":}";
    else
        stream << "{" << lower << " : " << upper << "}";
}

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
std::ostream &operator<<(std::ostream &stream, const Term &term) {
    if(term.predicate == Term::Predicate::AND)
        stream << '&';
    else if(term.predicate == Term::Predicate::NOT)
        stream << '!';

    // Overloaded function, will delegate to inherited type
    term.print(stream);

    // Print bounds (if there are any)
    print_bounds(stream, term.bounds);

    return stream;
}

}; // Namespace Pimlico
