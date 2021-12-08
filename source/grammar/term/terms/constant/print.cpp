#include "constant.hpp"

namespace Pimlico {
    
/* Print a term instance

Arguments
---------
stream
    the stream to print to
*/
void Constant::print(std::ostream &stream) const {
    static const std::map<char, std::string> escape_codes = {
        {'\n', "\\n"},
        {'\r', "\\r"} ,
        {'\b', "\\b"},
        {'\t', "\\t"},
        {'\\', "\\\\"},
        {'\"', "\\\""},
        {'\'', "\\\'"},
    };

    stream << '\'';

    for(const char &character : this->value) {
        if(escape_codes.find(character) != escape_codes.end())
            stream << escape_codes.at(character);
        else
            stream << character;
    }

    stream << '\'';
}

}; // Namespace Pimlico
