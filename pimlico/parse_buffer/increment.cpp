#include "parse_buffer.hpp"

#include "../text_position/text_position.hpp"

/* Increments the buffer's position

Will also update values like the line and column number, the current
indentation, and whether the line is broken
*/
void ParseBuffer::increment() {
    if(finished())
        return;

    position.index += 1;
    char character = get_character();

    TextPosition &position = this->position;

    // Handle newlines
    if(character == '\n') {

        // This target, if met, means the line is unbroken
        int indentation_target = position.block_indentation + 8;
        int indentation = this->line_indices[position.line].second;
        int line_count = line_indices.size();

        // Check if the line is not the last one, and if the target is met
        position.line_broken = false;
        if((position.line < line_count) && (indentation >= indentation_target))
            position.line_broken = true;

        // Otherwise, recalculate the current indentation
        else if(position.line < line_count) {
            position.block_indentation =
                    this->line_indices[position.line].second;
        }

        // Increment the line, column values
        position.line += 1;
        position.column = 1;
    }

    // Handle horizontal tabs
    else if(this->text[position.index] == '\t') {
        int remainder = position.column % 5;
        position.column += (5 - remainder);
    }

    // Handle carriage-returns
    else if(this->text[position.index] != '\r')
        position.column += 1;
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
