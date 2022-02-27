#include "form.hpp"

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

void Form::substitute(const std::string &token, const std::string &value) {
    this->substitutions[token] = value;
}

};

};
