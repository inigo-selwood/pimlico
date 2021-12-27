#include <catch.hpp>

#include <grammar/term/terms/range/range.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.range:parse") {

    SECTION("simple") {
        Buffer::Parse buffer("['a' - 'z']");
        Buffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range);
        REQUIRE(buffer.finished());

        delete range;
    }

    SECTION("invalid-range") {
        Buffer::Parse buffer("['z' - 'a']");
        Buffer::Error errors;

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

        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-seperator") {
        Buffer::Parse buffer("['a''z']");
        Buffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-closing-bracket") {
        std::string text = GENERATE(as<std::string>{},
                "['a'-'z'",
                "['a'-'z'\n]");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *range = Range::parse(buffer, errors);
        REQUIRE(range == nullptr);
        REQUIRE(errors);
    }
}
