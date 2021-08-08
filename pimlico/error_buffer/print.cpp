#include "error_buffer.hpp"

std::ostream &operator<<(std::ostream &stream, const ErrorBuffer &buffer) {
    buffer.print(stream);
    return stream;
}

void ErrorBuffer::print(std::ostream &stream) const {
    // stream << "error count: " << this->errors.size() << '\n';
    for(const auto &error : this->errors)
        stream << error.first << ' ' << error.second << '\n';
}