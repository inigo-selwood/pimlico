#include "constant.hpp"

namespace Pimlico {

/* Parses a constant term

Constants have the following format:

`'this is a constant`

Arguments
---------
buffer
    the buffer to parse the constant from
errors
    buffer for error reporting

Returns
-------
term
    a populated Constant instance, or nullptr if an error is encountered
*/
Constant *Constant::parse(ParseBuffer &buffer, ParseBuffer::Error &errors) {
    if(buffer.read('\'') == false)
        throw "no constant found";

    // Create the constant
    Constant *constant = new Constant();
    constant->type = Term::Type::CONSTANT;
    constant->position = buffer.position;

    // Parse the constant
    while(true) {

        // Stop parsing if the end of the constant is reached
        if(buffer.read('\''))
            break;

        // Handle unexpected EOFs
        if(buffer.finished()) {
            errors.add("constant.parse",
                    "unexpected end-of-file in constant",
                    buffer);
            delete constant;
            return nullptr;
        }

        // Handle unexpected EOLs
        const char character = buffer.peek();
        if(character == '\n' || character == '\r') {
            errors.add("constant.parse",
                    "unexpected end-of-line in constant",
                    buffer);
            delete constant;
            return nullptr;
        }

        // Check the character is valid
        else if(character < ' ' || character > '~') {
            errors.add("constant.parse",
                    "invalid character in constant",
                    buffer);
            delete constant;
            return nullptr;
        }

        // // Parse escape codes
        // else if(buffer.peek('\\')) {
        //     char escape_code = parse_escape_code(buffer, errors);
        //     if(escape_code == 0)
        //         return nullptr;
        //
        //     constant->value += escape_code;
        // }

        // Barring any of the above, add the raw value
        else
            constant->value += buffer.read();
    }

    // Check the constant wasn't empty
    if(constant->value.empty()) {
        errors.add("constant.parse",
                "empty constant",
                buffer,
                constant->position);
        delete constant;
        return nullptr;
    }

    return constant;
}

}; // Namespace Pimlico
