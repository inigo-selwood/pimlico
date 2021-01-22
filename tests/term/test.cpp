#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "text_buffer.hpp"
#include "syntax_error.hpp"

#include "pimlico.hpp"
using namespace Pimlico;

int main(int argument_count, char *argument_values[]) {
    if(argument_count != 2)
        return 1;

    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false)
        return 2;

    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    std::vector<SyntaxError> errors;
    TextBuffer buffer(grammar_string);

    try {
        const auto term = Term::parse(buffer, errors, true);
        if(term == nullptr) {
            for(const SyntaxError &error : errors)
                std::cerr << error << "\n";
            return 1;
        }

        std::cout << "output: " << *term << "\n";

        return 0;
    }
    catch(ParseLogicError &exception) {
        std::cerr << exception.what() << "\n";
        for(const SyntaxError &error : errors)
            std::cerr << error << "\n";
        return 2;
    }
}
