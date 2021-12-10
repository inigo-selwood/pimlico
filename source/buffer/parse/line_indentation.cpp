#include <iostream>

#include "parse.hpp"

namespace Pimlico {

namespace Buffer {

/* Gets the indentation level of the current (or given) line

Arguments
---------
line_number: long
    line number of which to get the indentation (0 by default, the current
    line). Note: line indices start at 1

Returns
-------
indentation: char
    the indentation level, where a tab counts as 4 instances
*/
char Parse::line_indentation(long line_number) const {

    // Assign the current line number, if the one given is zero
    if(line_number <= 0)
        line_number = this->position.line;

    // Check the line number is within bounds
    if(line_number < 1 || line_number > this->indextations.size())
        return 0;

    return this->indextations[line_number - 1].second;
}

}; // Namespace Buffer

}; // Namespace Pimlico
