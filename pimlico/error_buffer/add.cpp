#include "error_buffer.hpp"

void ErrorBuffer::add(const std::string &message, 
        const TextPosition &position) {
    
    this->errors.push_back({message, position});
}