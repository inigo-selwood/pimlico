#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:peek") {

    // Peek for an empty buffer should return 0
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.peek() == 0);
    }

    // Try a valid character, check the buffer's position hasn't changed
    SECTION("check-character") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek() == 'a');
        REQUIRE(buffer.position == start_position);
    }

    // Alternatively, try checking for the presence of a given character rather
    // than reading it -- check the position stays the same
    SECTION("verify-character-present-true") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('a'));
        REQUIRE(buffer.position == start_position);
    }

    // Make sure peek doesn't confirm presence of characters which aren't there
    SECTION("verify-character-present-false") {
        Buffer::Parse buffer("a");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('b') == false);
        REQUIRE(buffer.position == start_position);
    }

    // Should return false if the string being peeked for would overrun the
    // length of the buffer
    SECTION("verify-string-present-overflow") {
        Buffer::Parse buffer("a short string");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("a much longer string") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Peek a string that does match, and ends where the buffer ends
    SECTION("verify-final-string-present-true") {
        Buffer::Parse buffer("hello world");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello world"));
        REQUIRE(buffer.position == start_position);
    }

    // Check a string short enough that it might be present, but actually isn't
    SECTION("verify-final-string-present-false") {
        Buffer::Parse buffer("hello world");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello earth") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Check a substring matches
    SECTION("verify-string-present-true") {
        Buffer::Parse buffer("the quick brown fox jumped over the lazy dog");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the quick brown fox"));
        REQUIRE(buffer.position == start_position);
    }

    // Check an invalid substring doesn't match
    SECTION("verify-string-present-false") {
        Buffer::Parse buffer("the quick brown fox jumped over the lazy dog");
        const Buffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the lazy dog") == false);
        REQUIRE(buffer.position == start_position);
    }
}
