#include "text_position.hpp"

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
std::ostream &operator<<(std::ostream &stream, const TextPosition &position) {
    position.print(stream);
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
void TextPosition::print(std::ostream &stream) const {
    stream << '[' << this->line << ':' << this->column << ']';  
}