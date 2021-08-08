#include "reference.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

/* Parses a Reference term

References have the following format:

`this_is_a_reference`

Arguments
---------
buffer
    the buffer to parse reference from
errors
    buffer for error reporting

Returns
-------
term
    a populated Reference instance, or nullptr if an error is encountered
*/
Reference *Reference::parse(ParseBuffer &buffer, ErrorBuffer &errors) {

    Reference *reference = new Reference();
    reference->position = buffer.position;

    while(true) {
        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z') ||
                character == '_' ||
                character == '-' ||
                character == '.')
            reference->name += buffer.read();
        else
            break;
    }

    if(reference->name.empty())
        throw "no reference found";

    return reference;
}
