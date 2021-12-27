#include <catch.hpp>

#include <grammar/term/terms/choice/choice.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.choice:parse") {

    SECTION("simple") {
        Buffer::Parse buffer("a | b");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice);
        REQUIRE(buffer.finished());
        
        delete choice;
    }

    SECTION("single-term") {
        Buffer::Parse buffer("a");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice);
        REQUIRE(buffer.finished());

        delete choice;
    }

    SECTION("invalid-term") {
        Buffer::Parse buffer("~");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-file") {
        Buffer::Parse buffer("a |");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished());
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-line") {
        Buffer::Parse buffer("a |\n");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }

    SECTION("unexpected-end-of-file") {
        Buffer::Parse buffer("a | )");
        Buffer::Error errors;

        Term *choice = Choice::parse(buffer, errors);
        REQUIRE(choice == nullptr);
        REQUIRE(buffer.finished() == false);
        REQUIRE(errors);
    }
}
