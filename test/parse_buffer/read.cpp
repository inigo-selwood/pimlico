#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:read") {

    // Should fail if the buffer is empty
    SECTION("empty") {
        ParseBuffer buffer("");
        REQUIRE(buffer.read() == 0);
    }

    // Returns character present in buffer
    SECTION("check-character") {
        ParseBuffer buffer("a");

        REQUIRE(buffer.read() == 'a');
        REQUIRE(buffer.finished());
    }

    // Test newlines -- should update the line and column counters
    SECTION("check-character-newline") {
        ParseBuffer buffer("\n");

        REQUIRE(buffer.read() == '\n');
        REQUIRE(buffer.position.column == 1);
        REQUIRE(buffer.position.line == 2);
        REQUIRE(buffer.finished());
    }

    // Test tabs -- should update the column counter
    SECTION("check-character-tab") {
        ParseBuffer buffer("\t");

        REQUIRE(buffer.read() == '\t');
        REQUIRE(buffer.position.column == 5);
        REQUIRE(buffer.finished());
    }

    // Verifies given character is in buffer
    SECTION("verify-character-present-true") {
        ParseBuffer buffer("a");

        REQUIRE(buffer.read('a'));
        REQUIRE(buffer.finished());
    }

    // Doesn't verify presence of character which isn't in buffer
    SECTION("verify-character-present-false") {
        ParseBuffer buffer("a");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.read('b') == false);
        REQUIRE(buffer.position == start_position);
    }

    // Shows string isn't present, where string is longer than length of buffer
    SECTION("verify-string-not-present-overflow") {
        ParseBuffer buffer("a short string");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.read("a much longer string") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Shows string is present in buffer
    SECTION("verify-final-string-present-true") {
        std::string text = "hello world";
        ParseBuffer buffer(text);

        REQUIRE(buffer.read(text));
        REQUIRE(buffer.finished());
    }

    // Shows string isn't present if doesn't match what's in buffer
    SECTION("verify-final-string-present-false") {
        ParseBuffer buffer("hello world");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.read("hello earth") == false);
        REQUIRE(buffer.position == start_position);
    }

    // Shows string is present if matches substring in buffer
    SECTION("verify-string-present-true") {
        ParseBuffer buffer("the quick brown fox jumped over the lazy dog");

        std::string test_string = "the quick brown fox";
        int string_length = test_string.length();

        ParseBuffer::Position end_position = buffer.position;
        end_position.column += string_length;
        end_position.index += string_length;

        REQUIRE(buffer.read(test_string));
        REQUIRE(buffer.position == end_position);
    }

    // Shows string isn't there if it isn't present in buffer, but has length
    // such that it could be
    SECTION("verify-string-present-false") {
        ParseBuffer buffer("the quick brown fox jumped over the lazy dog");
        const ParseBuffer::Position start_position = buffer.position;

        REQUIRE(buffer.read("the lazy dog") == false);
        REQUIRE(buffer.position == start_position);
    }
}
