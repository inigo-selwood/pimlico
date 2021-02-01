#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "pimlico.hpp"
using namespace Pimlico;

enum Error {
    NONE = 0,

    WRONG_ARGUMENT_COUNT,
    INVALID_FILENAME,
    PARSE_SYNTAX_ERROR,
    INCOMPLETE_PARSE,
    PARSE_LOGIC_ERROR,
};

int main(int argument_count, char *argument_values[]) {

    // Check there was a filename specified
    if(argument_count != 2) {
        std::cerr << "wrong number of arguments\n";
        return Error::WRONG_ARGUMENT_COUNT;
    }
    // Open the grammar stream
    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false) {
        std::cerr << "invalid filename\n";
        return Error::INVALID_FILENAME;
    }

    // Place the stream's contents into a string
    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    // Parse the grammar term
    std::vector<TextBuffer::SyntaxError> errors;
    TextBuffer buffer(grammar_string);
    try {
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            for(const TextBuffer::SyntaxError &error : errors)
                std::cerr << error << "\n";
            return Error::PARSE_SYNTAX_ERROR;
        }

        // Print the term
        std::cout << *term << "\n";

        // Check the EOF was reached
        buffer.skip_whitespace();
        if(buffer.end_reached() == false) {
            std::cerr << "incomplete parse\n";
            return Error::INCOMPLETE_PARSE;
        }

        return 0;
    }

    // Handle exceptions
    catch(ParseLogicError &exception) {
        std::cerr << exception << "\n";
        for(const TextBuffer::SyntaxError &error : errors)
            std::cerr << error << "\n";
        return Error::PARSE_LOGIC_ERROR;
    }
}
