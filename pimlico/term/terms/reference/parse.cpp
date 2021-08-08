#include "reference.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

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
