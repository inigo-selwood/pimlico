#include "parse_buffer.hpp"

#include <utility>
#include <vector>

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
    for(unsigned int index = 0; index < length; index += 1) {

        // Evaluate indentation
        unsigned int indentation = 0;
        while(true) {
            if(index == text.size())
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
        while(index < length && text[index] != '\n')
            index += 1;

        // Store index, indentation
        line_indices.push_back({index, indentation});
    }

    // Add an empty line if the text is newline-terminated
    if(text[length - 1] == '\n')
        line_indices.push_back({length - 1, 0});

    return line_indices;
}

ParseBuffer::ParseBuffer(const std::string &text) {
    this->text = text;
    this->length = text.length();
    this->line_indices = evaluate_line_indices(text);
    this->position = TextPosition(this->line_indices);
}
