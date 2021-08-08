#include "text_position.hpp"

std::ostream &operator<<(std::ostream &stream, const TextPosition &position) {
    position.print(stream);
    return stream;
}

void TextPosition::print(std::ostream &stream) const {
    stream << '[' << this->line << ':' << this->column << ']';  
}