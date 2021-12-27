#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/choice/choice.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.choice:print") {

    SECTION("simple") {
        std::string text = "a | b | c";

        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }

    SECTION("enclosed") {
        std::string text = "(a | b)+";

        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }

    SECTION("enclosed-child") {
        std::string text = "(a b) | c";

        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);

        std::stringstream stream;
        stream << *choice;

        REQUIRE(stream.str() == text);

        delete choice;
    }
}
