#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "pimlico.hpp"

enum Error {
    NONE = 0,

    WRONG_ARGUMENT_COUNT    = 1,
    INVALID_FILENAME        = 2,
    PARSE_SYNTAX_ERROR      = 3,
    PARSE_LOGIC_ERROR       = 4,
    INCOMPLETE_PARSE        = 5,
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
    ErrorBuffer errors;
    ParseBuffer buffer(grammar_string);
    try {
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            errors.print(std::cerr);
            return Error::PARSE_SYNTAX_ERROR;
        }

        // Print the term
        std::cout << *term << "\n";

        // Check the EOF was reached
        buffer.skip(false);
        if(buffer.finished() == false) {
            errors.add("incomplete parse", buffer.position);
            errors.print(std::cerr);
            return Error::INCOMPLETE_PARSE;
        }

        delete term;

        return 0;
    }

    catch(const char *exception) {
        errors.print(std::cout);
        std::cout << "exception: " << exception << '\n';
        return Error::PARSE_LOGIC_ERROR;
    }

    // Handle exceptions
    // catch(ParseLogicError &exception) {
    //     std::cerr << exception << "\n";
    //     // for(const TextBuffer::SyntaxError &error : errors)
    //     //     std::cerr << error << "\n";
    //     return Error::PARSE_LOGIC_ERROR;
    // }
}
