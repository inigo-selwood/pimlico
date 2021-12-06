#include "choice.hpp"

namespace Pimlico {

/* Parses a choice term

Choices have the following format:

`term | other_term | a_third_term`

Arguments
---------
buffer
    the buffer to parse choice from
errors
    buffer for error reporting

Returns
-------
term
    a populated Choice instance, or nullptr if an error is encountered
*/
Term *Choice::parse(Buffer::Parse &buffer, Buffer::Error &errors) {

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
        buffer.skip_space();
        if(buffer.read('|') == false)
            break;

        // Report an error if an invalid state is encountered after the choice
        // operator
        buffer.skip_space();
        if(buffer.finished()) {
            errors.add("unexpected end-of-file in choice", buffer);
            return nullptr;
        }
        else if(buffer.peek('\n')) {
            errors.add("unexpected end-of-line in choice", buffer);
            return nullptr;
        }
        else if(buffer.peek(')')) {
            errors.add("unexpected ')' in choice", buffer);
            return nullptr;
        }
    }

    // If the choice only has one option, return that instead
    if(choice->values.size() == 1)
        return choice->values.back();

    return choice;
}

}; // Namespace Pimlico
