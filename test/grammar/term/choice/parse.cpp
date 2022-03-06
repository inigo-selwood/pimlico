#include <catch.hpp>

#include <grammar/term/terms/choice/choice.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.choice:parse") {

    SECTION("simple") {
        ParseBuffer buffer("a | b");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice);
        REQUIRE(buffer.finished());

        delete choice;
    }

    SECTION("single-term") {
        ParseBuffer buffer("a");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice);
        REQUIRE(buffer.finished());

        delete choice;
    }

    SECTION("invalid-term") {
        ParseBuffer buffer("~");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-file") {
        ParseBuffer buffer("a |");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished());
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-line") {
        ParseBuffer buffer("a |\n");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-file") {
        ParseBuffer buffer("a | )");
        ParseBuffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }
}
