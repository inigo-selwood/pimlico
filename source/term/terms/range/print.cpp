#include "range.hpp"

namespace Pimlico {

/* Print a term instance

Arguments
---------
stream
    the stream to print to
*/
void Range::print(std::ostream &stream) const {
    stream << "[\'";

    static const std::map<char, std::string> escape_codes = {
        {'\n', "\\n"},
        {'\r', "\\r"} ,
        {'\b', "\\b"},
        {'\t', "\\t"},
        {'\\', "\\\\"},
        {'\"', "\\\""},
        {'\'', "\\\'"},
    };

    const std::array<char, 2> &range = this->value;
    if(escape_codes.find(range[0]) != escape_codes.end())
        stream << escape_codes.at(range[0]);
    else
        stream << range[0];

    stream << "' - '";

    if(escape_codes.find(range[1]) != escape_codes.end())
        stream << escape_codes.at(range[1]);
    else
        stream << range[1];

    stream << "\']";
}

}; // Namespace Pimlico
