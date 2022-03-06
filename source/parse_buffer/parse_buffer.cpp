#include "parse_buffer.hpp"

namespace Pimlico {

/* Calculate line start index and indentation for each line
Arguments
---------
text
    the text to search
Returns
-------
index_indices
    (index, indentation) pairs for each line
*/
static std::vector<std::pair<long, char>>
        read_index_indentations(const std::string &text) {

    // Memoize length
    long length = text.length();

    // Iterate lines
    std::vector<std::pair<long, char>> index_indentations;
    long line_start_index = 0;
    for(long index = 0; index < length; index += 1) {
        line_start_index = index;

        // Evaluate indentation
        int indentation = 0;
        while(true) {
            if(index == length)
                break;

            if(text[index] == '\t')
                indentation =  ((indentation + 4) & ~0x03);
            else if(text[index] == ' ')
                indentation += 1;
            else
                break;

            index += 1;
        }

        // Add the (line-start, indentation) pair to the vector
        const std::pair<long, char> index_indentation = {
            line_start_index,
            indentation
        };
        index_indentations.push_back(index_indentation);

        // Move to end-of-line
        while(index <= length && text[index] != '\n')
            index += 1;
    }

    return index_indentations;
}

// Constructor
ParseBuffer::ParseBuffer(const std::string &text) {
    this->text = text;
    this->length = text.length();
    this->indextations = read_index_indentations(text);

    const bool empty = this->length == 0;
    this->position = ParseBuffer::Position(empty);
}

}; // Namespace Pimlico
