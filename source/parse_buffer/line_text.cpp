#include <map>
#include <string>

#include "parse_buffer.hpp"

/* Replace instances of format characters in a body of text with escape codes

Arguments
---------
text 
    the text to emplace codes in
*/
void emplace_escape_codes(std::string &text) {

    // Map of escape codes 
    // To-do: this map now exists in three places, it'd be better to centralize 
    // in a dedicated header
    static const std::map<char, std::string> escape_codes = {
        {'\n', "\\n"},
        {'\r', "\\r"} ,
        {'\b', "\\b"},
        {'\t', "\\t"},
        {'\\', "\\\\"},
        {'\"', "\\\""},
        {'\'', "\\\'"},
    };

    // Emplace the escape codes
    std::string result;
    for(const auto &character : text) {
        if(escape_codes.find(character) != escape_codes.end())
            result += escape_codes.at(character);
        else
            result += character;
    }

    // Reassign the reference
    text = result;
}

/* Get the text on a given line

Arguments
---------
line
    the number of the line to get the text on
escaped
    if true, substitutes escape characters with their backslashed equivalents

Returns
-------
text
    the text on the given line, or "" if the line was out of range
*/
std::string ParseBuffer::line_text(const long &line_number,
        const bool &escaped) const {
    
    // Check the line number is in range
    if(line_number < 1 || line_number > this->line_indices.size())
        return "";
    
    // Work out the start and end indices of the line
    long start = this->line_indices.at(line_number - 1).first;
    long end = this->line_indices.at(line_number).first;

    // Extract the substring, and optionally emplace escape codes
    std::string text = this->text.substr(start, end - start);
    if(escaped)
        emplace_escape_codes(text);
    
    return text;
}

/* Get the text on the current line

Arguments
---------
escaped
    if true, substitutes escape characters with their backslashed equivalents

Returns
-------
text
    the text on the current line
*/
std::string ParseBuffer::line_text(const bool &escaped) const {
    return line_text(this->position.line, escaped);
}
