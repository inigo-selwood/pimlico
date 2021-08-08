#include "rule.hpp"

#include "../term/term.hpp"

#include "../error_buffer/error_buffer.hpp"
#include "../parse_buffer/parse_buffer.hpp"

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

    buffer.skip(false);
    if(buffer.read(':') == false) {
        errors.add("expected ':'", buffer.position);
        buffer.skip_line(true);
        return nullptr;
    }

    buffer.skip(true);
    rule->value = Term::parse(buffer, errors, true);
    if(rule->value == nullptr) {
        buffer.skip(true);
        return nullptr;
    }

    return rule;
}
