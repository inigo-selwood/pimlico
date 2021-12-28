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

        std::stringstream stream;
        stream << *production;

        REQUIRE(production);
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

        std::stringstream stream;
        stream << *production;

        REQUIRE(production);
        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete production;
    }
}
