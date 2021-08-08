#include "../../parse_buffer/parse_buffer.hpp"
#include "../../error_buffer/error_buffer.hpp"

static char parse_escape_code(ParseBuffer &buffer, ErrorBuffer &errors) {
    
    // Check escape character present
    if(buffer.read('\\') == false)
        throw "parse_escape_code called with no code";

    // Extract character
    switch(buffer.read()) {
        case '\'':
            return '\'';
        case '\"':
            return '\"';
        case '\\':
            return '\\';
        case 'b':
            return '\b';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';

        default:
            errors.add("invalid escape code", buffer.position);
            return 0;
    }
}