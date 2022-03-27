#include "reference.hpp"

namespace Pimlico {

bool Reference::emplaceRules(std::map<std::string, Rule *> &rules,
        ParseBuffer::Error &errors) {

    if(rules.find(this->name) == rules.end()) {
        errors.add("refereence.emplace-rules",
                "no rule with name '" + this->name + "'");
        return false;
    }

    return true;
}


}; // Namespace Pimlico
