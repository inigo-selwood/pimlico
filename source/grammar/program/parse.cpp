#include "program.hpp"

namespace Pimlico {

Program *Program::parse(const std::string &grammar, Buffer::Error &errors) {
    Buffer::Parse buffer(grammar);

    Program *program = new Program();

    bool errors_found = false;
    while(true) {
        buffer.skip_whitespace();
        if(buffer.finished())
            break;

        Rule *rule = Rule::parse(buffer, errors);
        if(buffer.finished() == false )
            throw "incomplete rule parse";
        else if(rule == nullptr) {
            errors_found = true;
            continue;
        }

        int name_hash = std::hash<std::string>{}(rule->name);
        if(program->rules.find(name_hash) == program->rules.end()) {
            errors.add("rule redefinition", buffer);
            errors_found = true;
            continue;
        }
    }

    if(errors_found)
        return nullptr;

    return program;
}

}; // Namespace Pimlico