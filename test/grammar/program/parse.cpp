#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.program:parse") {

    SECTION("basic") {
        std::string grammar =
            "rule_one := term\n"
            "\n"
            "rule_two :=\n"
            "    term_one\n"
            "    term_two\n";
        Buffer::Error errors;

        Program *program = Program::parse(grammar, errors);
        INFO(errors);
        REQUIRE(program);

        delete program;
    }
}
