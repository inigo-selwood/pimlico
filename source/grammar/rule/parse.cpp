#include "rule.hpp"

namespace Pimlico {

/* Parses a rule

Arguments
---------
buffer
    the buffer to parse the rule from
errors
    a buffer for error reporting

Returns
-------
rule
    the rule parsed, or nullptr if an error is encountered

Throws
------
parse_logic_exception
    if parse_rule was called at an invalid position, or any of its child terms
    are called in error
*/
Rule *Rule::parse(Buffer::Parse &buffer, Buffer::Error &errors) {

    // Create a new rule at the current position
    Rule *rule = new Rule();
    if(rule == nullptr)
        return nullptr;
    rule->position = buffer.position;

    // Parse the rule's name (all lowercase/underscore)
    while(true) {
        char character = buffer.peek();
        if((character >= 'a' && character <= 'z') ||
                character == '_')
            rule->name += character;
        else
            break;
    }

    // Check a name was found where one was expected
    if(rule->name.empty()) {
        delete rule;
        throw "no rule found";
    }

    // Parse the return type, if there is one
    buffer.skip_space();
    if(buffer.read('<')) {

        // Keep reading until the end of the type embedding is reached
        char stack = 1;
        while(true) {
            if(buffer.finished())
                break;

            if(buffer.peek('<'))
                stack += 1;
            else if(buffer.peek('>'))
                stack -= 1;

            if(stack == 0)
                break;

            rule->type += buffer.read();
        }

        // Check there was a closing angle-bracket for the embedded type
        if(buffer.read('>') == false) {
            errors.add("no closing '>' for rule type", buffer);
        }
    }

    // Check for rule-production separator
    buffer.skip_space();
    if(buffer.read(":=") == false) {
        errors.add("expected ':='", buffer);
        delete rule;
        return nullptr;
    }

    // Check for newline after ':='
    buffer.skip_space();
    if(buffer.read('\n') == false) {
        errors.add("expected newline after ':='", buffer);
        delete rule;
        return nullptr;
    }

    // Parse productions
    while(true) {

        // Check the buffer isn't finished
        buffer.skip_space();
        if(buffer.finished())
            break;

        // Check we're still parsing a production (all productions below a rule
        // have to be at an indentation level of 4)
        if(buffer.line_indentation() != 4)
            break;

        // Parse a production at the current position
        Production *production = Production::parse(buffer, errors);
        if(production == nullptr) {
            delete rule;
            return nullptr;
        }
        rule->productions.push_back(production);

        buffer.skip_space();
    }

    return rule;
}

}; // Namespace Pimlico
