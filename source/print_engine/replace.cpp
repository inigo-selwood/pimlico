#include "print_engine.hpp"

namespace Pimlico {

void PrintEngine::replace(const std::string &token, const std::string &value) {
    this->substitutions[token] = value;
}

};
