#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "pimlico.hpp"
using namespace Pimlico;

int main(int argument_count, char *argument_values[]) {
    if(argument_count != 2)
        return 5;

    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false)
        return 4;

    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    std::vector<TextBuffer::SyntaxError> errors;
    TextBuffer buffer(grammar_string);

    try {
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            for(const TextBuffer::SyntaxError &error : errors)
                std::cerr << error << "\n";
            return 1;
        }

        std::cout << *term << "\n";

        buffer.skip_whitespace();
        if(buffer.end_reached() == false) {
            std::cerr << "incomplete parse\n";
            return 2;
        }

        return 0;
    }
    catch(ParseLogicError &exception) {
        std::cerr << exception << "\n";
        for(const TextBuffer::SyntaxError &error : errors)
            std::cerr << error << "\n";
        return 3;
    }
}
