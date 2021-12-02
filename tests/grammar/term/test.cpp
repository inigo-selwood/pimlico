#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "pimlico.hpp"

enum ErrorCode {
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
        return ErrorCode::WRONG_ARGUMENT_COUNT;
    }
    // Open the grammar stream
    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false) {
        std::cerr << "invalid filename\n";
        return ErrorCode::INVALID_FILENAME;
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
            std::cerr << errors << '\n';
            delete term;
            return ErrorCode::PARSE_SYNTAX_ERROR;
        }

        // Check the EOF was reached
        else if(buffer.finished() == false) {
            errors.add("incomplete parse");
            std::cerr << errors << '\n';
            delete term;
            return ErrorCode::INCOMPLETE_PARSE;
        }

        // Print the term
        std::cout << *term << "\n";

        delete term;
        return 0;
    }

    catch(const char *exception) {
        std::cerr << errors << '\n';
        std::cout << "exception: " << exception << '\n';
        return ErrorCode::PARSE_LOGIC_ERROR;
    }

    // Handle exceptions
    // catch(ParseLogicErrorCode &exception) {
    //     std::cerr << exception << "\n";
    //     // for(const TextBuffer::SyntaxErrorCode &error : errors)
    //     //     std::cerr << error << "\n";
    //     return ErrorCode::PARSE_LOGIC_ERROR;
    // }
}
