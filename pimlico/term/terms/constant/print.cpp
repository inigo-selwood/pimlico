#include <map>
#include <ostream>

#include "constant.hpp"

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
