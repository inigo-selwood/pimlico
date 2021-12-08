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
                character == '-' ||
                character == '.')
            rule->name += character;
        else
            break;
    }

    if(rule->name.empty())
        throw "no rule found";

    buffer.skip_space();
    if(buffer.read(':') == false) {
        errors.add("expected ':'", buffer);
        buffer.skip_line();
        return nullptr;
    }

    buffer.skip_space();
    rule->value = Term::parse(buffer, errors, true);
    if(rule->value == nullptr) {
        buffer.skip_space();
        return nullptr;
    }

    return rule;
}

}; // Namespace Pimlico
