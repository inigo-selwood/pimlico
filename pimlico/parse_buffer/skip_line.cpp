#include "parse_buffer.hpp"

void ParseBuffer::skip_line(const bool &overflow) {
    while(true) {
        skip(overflow);
        if(finished() || this->text[this->position.index - 1] == '\n')
            return;

        increment();
    }
}