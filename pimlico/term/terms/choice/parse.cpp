#include "choice.hpp"

#include "../../../error_buffer/error_buffer.hpp"
#include "../../../parse_buffer/parse_buffer.hpp"

Term *Choice::parse(ParseBuffer &buffer, ErrorBuffer &errors) {

    // Create a choice term
    Choice *choice = new Choice();
    choice->position = buffer.position;

    while(true) {

        // Parse option
        Term *value = Term::parse(buffer, errors, false);
        if(value == nullptr)
            return nullptr;
        choice->values.push_back(value);

        // Stop parsing terms if the end-of-file or end-of-line have been
        // reached, or if there's no pipe character
        buffer.skip(true);
        if(buffer.read('|') == false)
            break;

        // Report an error if an invalid state is encountered after the choice
        // operator
        buffer.skip(true);
        if(buffer.finished()) {
            errors.add("unexpected end-of-file in choice", buffer.position);
            return nullptr;
        }
        else if(buffer.peek('\n')) {
            errors.add("unexpected end-of-line in choice", buffer.position);
            return nullptr;
        }
        else if(buffer.peek(')')) {
            errors.add("unexpected ')' in choice", buffer.position);
            return nullptr;
        }
    }

    // If the choice only has one option, return that instead
    if(choice->values.size() == 1)
        return choice->values.back();

    return choice;
}
