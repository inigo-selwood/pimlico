#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Increments the buffer's position

Will also update values like the line and column number, the current
indentation, and whether the line is broken
*/
void Parse::increment() {
    Buffer::Position &position = this->position;
    if(position.index == this->length)
        return;

    position.index += 1;

    if(position.index < this->length) {
        switch(this->text[position.index]) {

            // Handle newlines, increment the line, column values
            case '\n':
                position.line += 1;
                position.column = 1;
                break;

            // Handle tabs, increase indentation up to the next multiple of 4
            case '\t':
                position.column = ((3 - position.column) & ~0x03) + 1;
                break;

            // Otherwise, (assuming the character is in the range [ -~],
            // increment the column
            default:
                if(this->text[position.index] >= ' '
                        && this->text[position.index] <= '~')
                    position.column += 1;
        }
    }
    else {
        position.column = 0;
        position.line = 0;
    }
}

/* Increment the buffer's position

Arguments
---------
steps
    the number of steps by which to increment the buffer's position
*/
void Parse::increment(const int &steps) {
    for(int step = 0; step < steps; step += 1)
        increment();
}

}; // Namespace Buffer

}; // Namespace Pimlico
