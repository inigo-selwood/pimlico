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

    WRONG_ARGUMENT_COUNT = 1,
    INVALID_FILENAME = 2,
    PARSE_SYNTAX_ERROR = 3,
    PARSE_LOGIC_ERROR = 4,
};

int main(int argument_count, char *argument_values[]) {
    if(argument_count != 2) {
        std::cerr << "wrong number of arguments\n";
        return Error::WRONG_ARGUMENT_COUNT;
    }

    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false) {
        std::cerr << "invalid filename\n";
        return Error::INVALID_FILENAME;
    }

    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    std::vector<TextBuffer::SyntaxError> errors;
    try {
        const auto specification = Specification::parse(grammar_string, errors);
        if(specification == nullptr) {
            for(const TextBuffer::SyntaxError &error : errors)
                std::cerr << error << "\n";
            return Error::PARSE_SYNTAX_ERROR;
        }

        std::cout << *specification << "\n";

        return 0;
    }
    catch(ParseLogicError &exception) {
        std::cerr << exception << "\n";
        for(const TextBuffer::SyntaxError &error : errors)
            std::cerr << error << "\n";
        return Error::PARSE_LOGIC_ERROR;
    }
}
