#include <catch.hpp>

#include <grammar/term/terms/range/range.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.range:parse") {

    SECTION("simple") {
        ParseBuffer buffer("['a' - 'z']");
        ParseBuffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range);
        REQUIRE(buffer.finished());

        delete range;
    }

    SECTION("invalid-range") {
        ParseBuffer buffer("['z' - 'a']");
        ParseBuffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-quotes") {
        std::string text = GENERATE(as<std::string>{},
                "[a'-'z']",
                "['a-'z']",
                "['a'-z']",
                "['a'-'z]");

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-seperator") {
        ParseBuffer buffer("['a''z']");
        ParseBuffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-closing-bracket") {
        std::string text = GENERATE(as<std::string>{},
                "['a'-'z'",
                "['a'-'z'\n]");
        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }
}
