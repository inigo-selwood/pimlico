#include "parse_buffer.hpp"

void ParseBuffer::skip(const bool &overflow) {
    while(true) {
        if(finished())
            return;

        // Handle spaces, tabs, line-feeds and newlines
        if(this->text[this->position.index] == ' '
                || this->text[this->position.index] == '\t'
                || this->text[this->position.index] == '\r'
                || (this->text[this->position.index] == '\n'
                    && this->position.line_broken
                    && overflow))
            increment();

        else
            break;
    }
}
