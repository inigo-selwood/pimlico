#include "sequence.hpp"

namespace Pimlico {

bool Sequence::emplaceRules(std::map<std::string, Rule *> &rules,
        ParseBuffer::Error &errors) {

    bool success = true;
    for(const auto &value : this->values)
        success &= value->emplaceRules(rules, errors);
    return success;
}


}; // Namespace Pimlico
