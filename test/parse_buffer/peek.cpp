#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:peek") {

    // Peek for an empty buffer should return 0
    SECTION("empty") {
        ParseBuffer buffer("");
        REQUIRE(buffer.peek() == 0);
    }

    // Try a valid character, check the buffer's position hasn't changed
    SECTION("check-character") {
        ParseBuffer buffer("a");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek() == 'a');
        REQUIRE(buffer.position == start_position);
    }

    // Alternatively, try checking for the presence of a given character rather
    // than reading it -- check the position stays the same
    SECTION("verify-character-present-true") {
        ParseBuffer buffer("a");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('a'));
        REQUIRE(buffer.position == start_position);
    }

    // Make sure peek doesn't confirm presence of characters which aren't there
    SECTION("verify-character-present-false") {
        ParseBuffer buffer("a");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek('b') == false);
        REQUIRE(buffer.position == start_position);
    }

    // Should return false if the string being peeked for would overrun the
    // length of the buffer
    SECTION("verify-string-present-overflow") {
        ParseBuffer buffer("a short string");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("a much longer string") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Peek a string that does match, and ends where the buffer ends
    SECTION("verify-final-string-present-true") {
        ParseBuffer buffer("hello world");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello world"));
        REQUIRE(buffer.position == start_position);
    }

    // Check a string short enough that it might be present, but actually isn't
    SECTION("verify-final-string-present-false") {
        ParseBuffer buffer("hello world");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("hello earth") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Check a substring matches
    SECTION("verify-string-present-true") {
        ParseBuffer buffer("the quick brown fox jumped over the lazy dog");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the quick brown fox"));
        REQUIRE(buffer.position == start_position);
    }

    // Check an invalid substring doesn't match
    SECTION("verify-string-present-false") {
        ParseBuffer buffer("the quick brown fox jumped over the lazy dog");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.peek("the lazy dog") == false);
        REQUIRE(buffer.position == start_position);
    }
}
