#pragma once

#include <string>

#include "../text_position/text_position.hpp"

class ErrorBuffer {

public:

    void add(const std::string &message, const TextPosition &position);

};
