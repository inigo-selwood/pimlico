#include "choice.hpp"

namespace Pimlico {

bool Choice::emplaceRules(std::map<std::string, Rule *> &rules,
        ParseBuffer::Error &errors) {

    bool success = true;
    for(const auto &value : this->values)
        success &= value->emplaceRules(rules, errors);
    return success;
}

}; // Namespace Pimlico
