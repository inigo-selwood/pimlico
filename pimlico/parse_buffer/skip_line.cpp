#include "parse_buffer.hpp"

/* Skips an entire line, up until a newline

Will skip newlines, if the line is broken, and if `overflow` is set 

Arguments 
---------
overflow
    if set, this value tells the function to continue skipping lines if the line 
    is broken (see [ParseBuffer](parse_buffer.cpp)), and the indentation has not 
    decreased
*/
void ParseBuffer::skip_line(const bool &overflow) {
    while(true) {
        skip(overflow);
        if(finished() || this->text[this->position.index - 1] == '\n')
            return;

        increment();
    }
}