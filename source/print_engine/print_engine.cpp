#include "print_engine.hpp"

namespace Pimlico {

PrintEngine::PrintEngine(const std::string &text) {
    this->indentation = 0;
    this->text = text;
}

};
