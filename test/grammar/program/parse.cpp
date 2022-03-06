#include <iostream>
#include <sstream>
#include <fstream>

#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.program:parse") {

    SECTION("register") {
        std::string file_name = GENERATE(as<std::string>{},
                "arithmetic-expression.peg",
                "identifier.peg",
                "radices.peg");

        std::ifstream file_stream("test/grammar/program/examples/" + file_name);
        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        std::string grammar = buffer.str();

        ParseBuffer::Error errors;

        Program *program = Program::parse(grammar, errors);

        INFO("filename: " << file_name);
        INFO("grammar:\n" << grammar);
        INFO("errors: " << errors);
        REQUIRE(program);

        delete program;
    }
}
