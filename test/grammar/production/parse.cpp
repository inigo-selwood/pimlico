#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.production:parse") {

    SECTION("basic") {
        ParseBuffer buffer("term");
        ParseBuffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }

    SECTION("invalid term") {
        ParseBuffer buffer("a |");
        ParseBuffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production == nullptr);
        REQUIRE(errors);
    }

    SECTION("unterminated-embedded-expression") {
        ParseBuffer buffer("a {");
        ParseBuffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production == nullptr);
        REQUIRE(errors);
    }

    SECTION("embedded-expression-nexted-brackets") {
        std::string text = "term {\n"
                "    class Foo {};\n"
                "}";
        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }

    SECTION("embedded-expression") {
        std::string text = "term {\n"
                "    std::cout << \"hello\\n\"\n"
                "}";
        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }
}
