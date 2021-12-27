#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:peek") {

    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.peek() == 0);
    }

    SECTION("check-character") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek() == 'a');
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-character-present-true") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('a'));
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-character-present-false") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('b') == false);
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-string-present-overflow") {
        Buffer::Parse buffer("a short string");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("a much longer string") == false);
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-final-string-present-true") {
        Buffer::Parse buffer("hello world");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello world"));
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-final-string-present-false") {
        Buffer::Parse buffer("hello world");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello earth") == false);
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-string-present-true") {
        Buffer::Parse buffer("the quick brown fox jumped over the lazy dog");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the quick brown fox"));
        REQUIRE(buffer.position == start_position);
    }

    SECTION("verify-string-present-false") {
        Buffer::Parse buffer("the quick brown fox jumped over the lazy dog");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the lazy dog") == false);
        REQUIRE(buffer.position == start_position);
    }
}
