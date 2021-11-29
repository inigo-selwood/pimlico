#include "parse_logic_exception.hpp"

ParseLogicException::ParseLogicException(const std::string &message, 
        const TextPosition &position) {
    
    this->message = message;
    this->position = position;
}