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

    Rule *rule = new Rule();
    rule->position = buffer.position;

    while(true) {
        char character = buffer.peek();
        if((character >= 'a' && character <= 'z') ||
                character == '_')
            rule->name += character;
        else
            break;
    }

    if(rule->name.empty())
        throw "no rule found";

    // Parse the return type, if there is one
    buffer.skip_space();
    if(buffer.read('<')) {

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

        if(buffer.read('>') == false) {
            errors.add("no closing '>' for rule type", buffer);
        }

    }

    return rule;
}

}; // Namespace Pimlico
