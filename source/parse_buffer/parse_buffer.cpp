#include <string>
#include <utility>
#include <vector>

#include <iostream>

#include "parse_buffer.hpp"

#include "../text_position/text_position.hpp"

/* Calculate line:(index, indentation) pairs for each line

Arguments
---------
text
    the text to evaluate

Returns
-------
lines
    (index, indentation) pairs indexed by line
*/
static std::vector<std::pair<long, int>> evaluate_line_indices(
        const std::string &text) {

    // Memoize length
    long length = text.length();

    // Iterate lines
    std::vector<std::pair<long, int>> line_indices;
    for(long index = 0; index < length; index += 1) {

        // Evaluate indentation
        int indentation = 0;
        while(true) {
            if(index == length)
                break;

            if(text[index] == '\t')
                indentation += 4;
            else if(text[index] == ' ')
                indentation += 1;
            else
                break;

            index += 1;
        }

        // Move to end-of-line
        while(index <= length && text[index] != '\n')
            index += 1;

        // Store index, indentation
        if(index)
            line_indices.push_back({index - 1, indentation});
        else 
            line_indices.push_back({0, 0});
    }

    return line_indices;
}

ParseBuffer::ParseBuffer(const std::string &text) {
    this->text = text;
    this->length = text.length();
    this->line_indices = evaluate_line_indices(text);
    this->position = TextPosition(this->line_indices);
}
