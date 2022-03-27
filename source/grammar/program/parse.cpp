#include "program.hpp"

namespace Pimlico {

Program *Program::parse(const std::string &grammar,
        ParseBuffer::Error &errors) {

    ParseBuffer buffer(grammar);

    Program *program = new Program();

    bool errors_found = false;
    while(true) {
        buffer.skip_whitespace();
        if(buffer.finished())
            break;

        // Parse the rule; check we've reached a newline (or the EOF)
        Rule *rule = Rule::parse(buffer, errors);
        if(buffer.finished() == false && buffer.peek('\n') == false)
            throw "incomplete rule parse";

        // If parsing the rule failed, skip lines until we reach 0 indentation
        // (or the end of the file)
        if(rule == nullptr) {
            while(true) {
                if(buffer.finished() || buffer.line_indentation() == 0)
                    break;

                buffer.skip_line();
            }

            errors_found = true;
            continue;
        }

        // Check that no rule with the same name has been defined already
        if(program->rules.find(rule->name) != program->rules.end()) {
            errors.add("program.parse", "rule redefinition", buffer);
            errors_found = true;
            delete rule;
            continue;
        }

        program->rules[rule->name] = rule;
    }

    // Check the parsed rules succeeded
    if(errors_found) {
        delete program;
        return nullptr;
    }

    // Now that we've built all the rules, we can emplace references to them
    bool success = true;
    for(std::pair<const std::string, Rule *> &pair : program->rules) {
        for(Production *production : pair.second->productions) {
            success &= production->value->emplaceRules(program->rules, errors);
        }
    }

    // Check the emplacements succeeded
    if(success == false) {
        delete program;
        return nullptr;
    }

    return program;
}

}; // Namespace Pimlico
