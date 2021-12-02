#pragma once

#include <string>

#include "../text_position/text_position.hpp"

struct ParseLogicException {

public:

    ParseLogicException(const std::string &message, 
            const TextPosition &position);

private:
    
    TextPosition position;

    std::string message;

};