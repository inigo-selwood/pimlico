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
            rule->name += buffer.read();
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

    bool parse_errors = false;
    while(true) {

        // Check the buffer isn't finished, that there's a newline, and that the
        // next line is properly indented
        buffer.skip_space();
        if(buffer.finished())
            break;

        // If there's a newline break, make sure the next line is indented
        // properly
        if(buffer.peek('\n')
                && buffer.line_indentation(buffer.position.line + 1) != 4)
            break;

        // Try to parse a production instance
        buffer.skip_whitespace();
        Production *production = Production::parse(buffer, errors);
        if(production == nullptr) {
            parse_errors = true;
            buffer.skip_line();
            continue;
        }
        rule->productions.push_back(production);
    }

    // Return any parse errors
    if(parse_errors) {
        delete rule;
        return nullptr;
    }

    // Check for errors
    if(rule->productions.empty()) {
        errors.add("no productions specified for rule", buffer);
        delete rule;
        return nullptr;
    }

    return rule;
}

}; // Namespace Pimlico
