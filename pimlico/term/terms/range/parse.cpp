#include "range.hpp"

#include "../parse_escape_code.hpp"

static char parse_character(ParseBuffer &buffer, ErrorBuffer &errors) {

    // Check for opening quote
    if(buffer.read('\'') == false) {
        errors.add("expected '\\\''", buffer.position);
        return 0;
    }

    // Handle escape codes
    char value = 0;
    if(buffer.peek('\\')) {
        char code = parse_escape_code(buffer, errors);
        if(code == -1)
            return 0;
        else if(code < ' ' || code > '~') {
            errors.add("escape code invalid in range", buffer.position);
            return 0;
        }

        value = code;
    }

    // Handle regular characters
    else
        value = buffer.read();

    // Check for closing quote
    if(buffer.read('\'') == false) {
        errors.add("expected '\\\''", buffer.position);
        return 0;
    }

    return value;
}


Range *Range::parse(ParseBuffer &buffer, ErrorBuffer &errors) {
    if(buffer.read('[') == false)
        throw "no range found";

    // Create the range object
    Range *range = new Range();
    range->position = buffer.position;

    // Parse the start value
    buffer.skip(false);
    char start_value = parse_character(buffer, errors);
    if(start_value == 0)
        return nullptr;

    // Check for the comma separator
    buffer.skip(false);
    if(buffer.read('-') == false) {
        errors.add("expected '-'", buffer.position);
        return nullptr;
    }

    // Parse the end value
    buffer.skip(false);
    char end_value = parse_character(buffer, errors);
    if(end_value == 0)
        return nullptr;

    // Check for a closing bracket
    buffer.skip(false);
    if(buffer.read(']') == false) {
        errors.add("expected ']'", buffer.position);
        return nullptr;
    }

    // Verify the range is a valid one
    if(start_value >= end_value) {
        errors.add("illogical range values", buffer.position);
        return nullptr;
    }

    range->values = std::array<char, 2>({start_value, end_value});
    return range;
}