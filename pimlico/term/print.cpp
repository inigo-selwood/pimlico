#include <ostream>

#include "term.hpp"

static void print_bounds(std::ostream &stream, const Term::Bounds &bounds) {
    int lower = bounds[0];
    int upper = bounds[1];

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

std::ostream &operator<<(std::ostream &stream, const Term &term) {
    if(term.predicate == Term::Predicate::AND)
        stream << '&';
    else if(term.predicate == Term::Predicate::NOT)
        stream << '!';

    // Overloaded function, will delegate to inherited type
    term.print(stream);

    print_bounds(stream, term.bounds);

    return stream;
}
