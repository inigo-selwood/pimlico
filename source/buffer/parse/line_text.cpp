#include <iostream>

#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Gets the text of the current (or given) line

Arguments
---------
line_number: long
    line number of which to get the indentation (0 by default, the current
    line). Note: line indices start at 1

Returns
-------
line_text: std::string
    the current line's text, or an empty string if the line number was invalid
*/
std::string Parse::line_text(long line_number) const {

    // Assign the current line number if the index given is zero) the default
    if(line_number <= 0)
        line_number = this->position.line;

    // Check the line number is in bounds
    if(line_number < 1 || line_number > this->indextations.size())
        return std::string();

    // Evaluate the start index (the index of the start of the given line)
    long start_index = 0;
    if(line_number > 1)
        start_index = this->indextations[line_number - 1].first;

    // Evaluate the end index (the index of the start of the next line, minus
    // one)
    long end_index = this->length;
    if(line_number < this->indextations.size())
        end_index = this->indextations[line_number].first - 1;
    else if(this->length && this->text[this->length - 1] == '\n')
        end_index = this->length - 1;

    // Get the length (end - start), and return the substring
    long length = end_index - start_index;
    return this->text.substr(start_index, length);
}

}; // Namespace Buffer

}; // Namespace Pimlico
