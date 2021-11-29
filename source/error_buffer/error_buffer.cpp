#include "error_buffer.hpp"

ErrorBuffer::~ErrorBuffer() {
    while(this->errors.empty() == false) {
        Error *error = this->errors.back();
        if(error)
            delete error;
        this->errors.pop_back();
    }
}
