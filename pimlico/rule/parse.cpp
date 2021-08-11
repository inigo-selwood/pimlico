#include "rule.hpp"

#include "../term/term.hpp"

#include "../error_buffer/error_buffer.hpp"
#include "../parse_buffer/parse_buffer.hpp"

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
Rule *Rule::parse(ParseBuffer &buffer, ErrorBuffer &errors) {

    Rule *rule = new Rule();
    rule->position = buffer.position;

    while(true) {
        char character = buffer.peek();
        if((character >= 'a' && character <= 'z') ||
                character == '_' ||
                character == '-' ||
                character == '.')
            rule->name += character;
        else
            break;
    }

    if(rule->name.empty())
        throw "no rule found";

    buffer.skip_space(false);
    if(buffer.read(':') == false) {
        errors.add("expected ':'", buffer);
        buffer.skip_line(true);
        return nullptr;
    }

    buffer.skip_space(true);
    rule->value = Term::parse(buffer, errors, true);
    if(rule->value == nullptr) {
        buffer.skip_space(true);
        return nullptr;
    }

    return rule;
}
