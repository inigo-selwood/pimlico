#include "../parse_buffer.hpp"

namespace Pimlico {

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
std::ostream &operator<<(std::ostream &stream,
        const ParseBuffer::Position &position) {

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
void ParseBuffer::Position::print(std::ostream &stream) const {
    stream << '[' << this->line << ':' << this->column << ']';
}

}; // Namespace Pimlico
