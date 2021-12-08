#include "production.hpp"

namespace Pimlico {

/* Parses a production

Arguments
---------
buffer
    the buffer to parse the production from
errors
    a buffer for error reporting

Returns
-------
production
    the production parsed, or nullptr if an error is encountered

Throws
------
parse_logic_exception
    if parse_rule was called at an invalid position, or any of its child terms
    are called in error
*/
Production *Production::parse(Buffer::Parse &buffer, Buffer::Error &errors) {

    Production *production = new Production();
    production->position = buffer.position;

    production->value = Term::parse(buffer, errors, true);
    if(production->value == nullptr) {
        buffer.skip_space();
        return nullptr;
    }

    buffer.skip_space();
    if(buffer.read('{')) {
        char stack = 0;
        while(true) {
            if(buffer.finished())
                break;

            if(buffer.peek('{'))
                stack += 1;
            else if(buffer.peek('{'))
                stack -= 1;

            if(stack == 0)
                break;

            production->expression += buffer.read();
        }

        if(buffer.read('}') == false)
            errors.add("no closing '}' for production expression");
    }

    return production;
}

}; // Namespace Pimlico
