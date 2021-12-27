#include "choice.hpp"

namespace Pimlico {

static void delete_values(std::vector<Term *> &values) {
    while(values.empty() == false) {
        if(values.back())
            delete values.back();

        values.pop_back();
    }
}

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

    std::vector<Term *> values;
    while(true) {

        // Parse option
        Term *value = Term::parse(buffer, errors, false);
        if(value == nullptr) {
            delete_values(values);
            delete choice;
            return nullptr;
        }
        values.push_back(value);

        // Stop parsing terms if the end-of-file or end-of-line have been
        // reached, or if there's no pipe character
        buffer.skip_space();
        if(buffer.read('|') == false)
            break;

        // Report an error if an invalid state is encountered after the choice
        // operator
        buffer.skip_space();
        bool error_encountered = false;
        if(buffer.finished()) {
            errors.add("unexpected end-of-file in choice", buffer);
            error_encountered = true;
        }
        else if(buffer.peek('\n')) {
            errors.add("unexpected end-of-line in choice", buffer);
            error_encountered = true;
        }
        else if(buffer.peek(')')) {
            errors.add("unexpected ')' in choice", buffer);
            error_encountered = true;
        }

        if(error_encountered) {
            delete_values(values);
            delete choice;
            return nullptr;
        }
    }

    // If the choice only has one option, return that instead
    if(values.size() == 1) {
        delete choice;
        return values.back();
    }

    choice->values = values;
    return choice;
}

}; // Namespace Pimlico
