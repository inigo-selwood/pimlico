#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/choice/choice.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.choice:print") {

    SECTION("simple") {
        std::string text = "a | b | c";

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }

    SECTION("enclosed") {
        std::string text = "(a | b)+";

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }

    SECTION("enclosed-child") {
        std::string text = "(a b) | c";

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }
}
