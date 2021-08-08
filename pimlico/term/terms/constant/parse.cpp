#include "constant.hpp"

#include "../parse_escape_code.hpp"

#include "../../term.hpp"

#include "../../../parse_buffer/parse_buffer.hpp"
#include "../../../error_buffer/error_buffer.hpp"

Constant *Constant::parse(ParseBuffer &buffer, ErrorBuffer &errors) {
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
            errors.add("invalid character in constant", buffer.position);
            return nullptr;
        }

        // Handle unexpected EOFs
        else if(buffer.finished()) {
            errors.add("unexpected end-of-file in constant", buffer.position);
            return nullptr;
        }

        // Handle unexpected EOLs
        else if(character == '\n' || character == '\r') {
            errors.add("unexpected end-of-line in constant", buffer.position);
            return nullptr;
        }

        // Parse escape codes
        else if(buffer.peek('\\')) {
            char escape_code = parse_escape_code(buffer, errors);
            if(escape_code == 0)
                return nullptr;

            constant->value += escape_code;
        }

        // Barring any of the above, add the raw value
        else
            constant->value += buffer.read();
    }

    // Check the constant wasn't empty
    if(constant->value.empty()) {
        errors.add("empty constant", buffer.position);
        return nullptr;
    }

    return constant;
}
