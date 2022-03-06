#include "parse_buffer.hpp"

namespace Pimlico {

/* Increments the buffer's position

Will also update values like the line and column number, the current
indentation, and whether the line is broken
*/
void ParseBuffer::increment() {
    ParseBuffer::Position &position = this->position;
    if(position.index >= this->length)
        return;

    switch(this->text[position.index]) {

        // Handle newlines, increment the line, column values
        case '\n':
            position.line += 1;
            position.column = 1;
            break;

        // Handle tabs, increase indentation up to the next multiple of 4
        case '\t':
            position.column = ((position.column + 3) / 4) * 4 + 1;
            break;

        // Otherwise, (assuming the character is in the range [ -~],
        // increment the column
        default:
            if(this->text[position.index] >= ' '
                    && this->text[position.index] <= '~')
                position.column += 1;
    }

    position.index += 1;
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

}; // Namespace Pimlico