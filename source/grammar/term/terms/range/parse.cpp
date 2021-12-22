#include "range.hpp"

namespace Pimlico {

/* Parse a range-character

Ranges contain two range-characters, where each looks like this:

`'a'`

Arguments
---------
buffer
    the buffer to parse the range character from
errors
    buffer for error reporting

Returns
-------
character
    the character found, or 0 if an error was encountered
*/
static char parse_character(Buffer::Parse &buffer, Buffer::Error &errors) {

    // Check for opening quote
    if(buffer.read('\'') == false) {
        errors.add("expected '\\\''", buffer);
        return 0;
    }

    // Handle escape codes
    // char value = 0;
    // if(buffer.peek('\\')) {
    //     char code = parse_escape_code(buffer, errors);
    //     if(code == -1)
    //         return 0;
    //     else if(code < ' ' || code > '~') {
    //         errors.add("escape code invalid in range", buffer);
    //         return 0;
    //     }
    //
    //     value = code;
    // }
    //
    // // Handle regular characters
    // else
    //     value = buffer.read();

    char value = buffer.read();

    // Check for closing quote
    if(buffer.read('\'') == false) {
        errors.add("expected '\\\''", buffer);
        return 0;
    }

    return value;
}

/* Parses a Range term

Ranges have the following format:

`['a' - 'z']`

Arguments
---------
buffer
    the buffer to parse range from
errors
    buffer for error reporting

Returns
-------
term
    a populated Range instance, or nullptr if an error is encountered
*/
Range *Range::parse(Buffer::Parse &buffer, Buffer::Error &errors) {
    if(buffer.read('[') == false)
        throw "no range found";

    // Create the range object
    Range *range = new Range();
    range->position = buffer.position;

    // Parse the start value
    buffer.skip_space();
    char start_value = parse_character(buffer, errors);
    if(start_value == 0) {
        delete range;
        return nullptr;
    }

    // Check for the comma separator
    buffer.skip_space();
    if(buffer.read('-') == false) {
        errors.add("expected '-'", buffer);
        delete range;
        return nullptr;
    }

    // Parse the end value
    buffer.skip_space();
    char end_value = parse_character(buffer, errors);
    if(end_value == 0) {
        delete range;
        return nullptr;
    }

    // Check for a closing bracket
    buffer.skip_space();
    if(buffer.read(']') == false) {
        errors.add("expected ']'", buffer);
        delete range;
        return nullptr;
    }

    // Verify the range is a valid one
    if(start_value >= end_value) {
        errors.add("illogical range values", buffer);
        delete range;
        return nullptr;
    }

    range->value = std::array<char, 2>({start_value, end_value});
    return range;
}

}; // Namespace Pimlico
