#include <string>
#include <map>
#include <functional>

#include "source.hpp"

#include "../error_buffer/error_buffer.hpp"
#include "../parse_buffer/parse_buffer.hpp"

Source *Source::parse(const std::string &grammar, ErrorBuffer &errors) {
    ParseBuffer buffer(grammar);

    Source *source = new Source();

    bool errors_found = false;
    while(true) {
        buffer.skip(false);
        if(buffer.finished())
            break;
        
        Rule *rule = Rule::parse(buffer, errors);
        if(buffer.finished() == false && buffer.peek('\n') == false)
            throw "incomplete rule parse";
        else if(rule == nullptr) {
            errors_found = true;
            continue;
        }
        
        int name_hash = std::hash<std::string>{}(rule->name);
        if(source->rules.find(name_hash) == source->rules.end()) {
            errors.add("rule redefinition", buffer.position);
            errors_found = true;
            continue;
        }
    }

    if(errors_found)
        return nullptr;

    return source;
}