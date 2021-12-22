#include "reference.hpp"

namespace Pimlico {

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
Reference *Reference::parse(Buffer::Parse &buffer, Buffer::Error &errors) {

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

    if(reference->name.empty()) {
        delete reference;
        throw "no reference found";
    }

    return reference;
}

}; // Namespace Pimlico
