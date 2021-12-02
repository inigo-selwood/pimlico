#include "parse_buffer.hpp"

#include "../text_position/text_position.hpp"

/* Increments the buffer's position

Will also update values like the line and column number, the current
indentation, and whether the line is broken
*/
void ParseBuffer::increment() {
    TextPosition &position = this->position;
    if(position.index == this->length)
        return;

    position.index += 1;

    switch(this->text[position.index]) {

        // Handle newlines 
        case '\n': {

            // Pull out some values for ease-of-reading
            int target = position.block_indentation + 8;
            int line_count = this->line_indices.size();
            int indentation = this->line_indices[position.line].second;
            
            // Check whether the line is broken
            if((position.line < line_count) && (indentation >= target))
                position.line_broken = true;
            else {
                position.line_broken = false;
                if(position.line < line_count)
                    position.block_indentation = indentation;
            }

            // Increment the line, column values
            position.line += 1;
            position.column = 0;
            break;
        }
        
        // Handle tabs 
        case '\t':
            position.column += (5 - position.column % 5);
            break;
        
        // Otherwise, (assuming the character is in the range [ -~], increment 
        // the column
        default:
            if(this->text[position.index] < ' ' 
                    || this->text[position.index] > '~')
                position.column += 1;
    }
}

/* Increment the buffer's position

Arguments
---------
steps
    the number of steps by which to increment the buffer's position
*/
void ParseBuffer::increment(const int &steps) {
    for(int step = 0; step < steps; step += 1)
        increment();
}
