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
Production *Production::parse(ParseBuffer &buffer,
        ParseBuffer::Error &errors) {

    // Create a production
    Production *production = new Production();
    if(production == nullptr)
        return nullptr;
    production->position = buffer.position;

    // Parse a term sequence
    Term *term = Term::parse(buffer, errors, true);
    if(term == nullptr) {
        buffer.skip_space();
        delete production;
        return nullptr;
    }
    production->value = term;

    // Check if there's an expression associated
    buffer.skip_space();
    if(buffer.read('{')) {
        char stack = 1;
        while(true) {
            if(buffer.finished())
                break;

            if(buffer.peek('{'))
                stack += 1;
            else if(buffer.peek('}'))
                stack -= 1;

            if(stack == 0)
                break;

            production->expression += buffer.read();
        }

        if(buffer.read('}') == false) {
            errors.add("production.parse",
                    "no closing '}' for production expression");
            return nullptr;
        }
    }

    return production;
}

}; // Namespace Pimlico
