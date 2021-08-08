#include "parse_buffer.hpp"

/* Skip whitespace characters (excluding newlines, unless `overflow` is set)

Arguments
---------
overflow
    if true, newlines on "broken" lines will be skipped too (for details on 
    broken lines, see [ParseBuffer](parse_buffer.cpp))
*/
void ParseBuffer::skip(const bool &overflow) {
    while(true) {
        if(finished())
        // if((this->position.index + 1) >= this->length)
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