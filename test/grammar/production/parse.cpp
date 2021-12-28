#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.production:parse") {

    SECTION("basic") {
        Buffer::Parse buffer("term");
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }

    SECTION("invalid term") {
        Buffer::Parse buffer("a |");
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production == nullptr);
        REQUIRE(errors);
    }

    SECTION("unterminated-embedded-expression") {
        Buffer::Parse buffer("a {");
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production == nullptr);
        REQUIRE(errors);
    }

    SECTION("embedded-expression-nexted-brackets") {
        std::string text = "term {\n"
                "    class Foo {};\n"
                "}";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }

    SECTION("embedded-expression") {
        std::string text = "term {\n"
                "    std::cout << \"hello\\n\"\n"
                "}";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        REQUIRE(production);
        REQUIRE(buffer.finished());

        delete production;
    }
}
