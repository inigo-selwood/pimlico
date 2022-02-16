#include <sstream>

#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.production:print") {

    SECTION("simple") {
        std::string text = "term";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        if(errors)
            INFO(errors);

        REQUIRE(production);
        std::stringstream stream;
        stream << *production;

        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete production;
    }

    SECTION("embedded-expression") {
        std::string text = "term {\n"
                "    class Foo {};\n"
                "}";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        if(errors)
            INFO(errors);

        REQUIRE(production);
        std::stringstream stream;
        stream << *production;

        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete production;
    }

    SECTION("bound-parameter-embedded-expression") {
        std::string text = "term <Namespace::Type> {\n"
                "    throw Exception();\n"
                "}";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Production *production = Production::parse(buffer, errors);
        INFO(errors);

        REQUIRE(production);
        std::stringstream stream;
        stream << *production;

        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete production;
    }
}
