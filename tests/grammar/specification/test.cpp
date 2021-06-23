#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "text_buffer.hpp"

#include "pimlico.hpp"
using namespace Pimlico;

enum Error {
    NONE = 0,

    WRONG_ARGUMENT_COUNT,
    INVALID_FILENAME,
    PARSE_SYNTAX_ERROR,
    PARSE_LOGIC_ERROR,
};

int main(int argument_count, char *argument_values[]) {

    // Check the right number of arugments are present
    if(argument_count != 2) {
        std::cerr << "wrong number of arguments\n";
        return Error::WRONG_ARGUMENT_COUNT;
    }

    // Open the grammar specification file
    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false) {
        std::cerr << "invalid filename\n";
        return Error::INVALID_FILENAME;
    }

    // Load the grammar file into a string
    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    // Try to parse the specification
    std::vector<TextBuffer::SyntaxError> errors;
    try {
        const auto specification = Specification::parse(grammar_string, errors);
        if(specification == nullptr) {
            for(const TextBuffer::SyntaxError &error : errors)
                std::cerr << error << "\n";
            return Error::PARSE_SYNTAX_ERROR;
        }

        // Print the parsed specification's contents
        std::cout << *specification << "\n";
        return 0;
    }

    // Deal with parsing errors
    catch(ParseLogicError &exception) {
        std::cerr << exception << "\n";
        for(const TextBuffer::SyntaxError &error : errors)
            std::cerr << error << "\n";
        return Error::PARSE_LOGIC_ERROR;
    }
}
