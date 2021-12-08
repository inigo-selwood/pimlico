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
Constant *Constant::parse(Buffer::Parse &buffer, Buffer::Error &errors) {
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

        // Check the character is valid
        const char character = buffer.peek();
        if(character < ' ' || character > '~') {
            errors.add("invalid character in constant", buffer);
            return nullptr;
        }

        // Handle unexpected EOFs
        else if(buffer.finished()) {
            errors.add("unexpected end-of-file in constant", buffer);
            return nullptr;
        }

        // Handle unexpected EOLs
        else if(character == '\n' || character == '\r') {
            errors.add("unexpected end-of-line in constant", buffer);
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
        errors.add("empty constant", buffer, constant->position);
        return nullptr;
    }

    return constant;
}

}; // Namespace Pimlico
