#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:line-text") {

    // Text for an empty buffer should be empty
    SECTION("empty") {
        ParseBuffer buffer("");
        REQUIRE(buffer.line_text() == "");
    }

    // Check line text works when the line is the entire buffer
    SECTION("non-empty") {
        ParseBuffer buffer("hello world");
        REQUIRE(buffer.line_text() == "hello world");
    }

    // Check the line text for a line outside of the buffer's range is
    // considered empty
    SECTION("out-of-range") {
        ParseBuffer buffer("hello world");
        REQUIRE(buffer.line_text(2) == "");
    }

    // Try providing the index of a specific line, the text of which to fetch
    SECTION("specific-line") {
        ParseBuffer buffer("\nhello world");
        REQUIRE(buffer.line_text(1) == "");
        REQUIRE(buffer.line_text(2) == "hello world");
        REQUIRE(buffer.line_text(3) == "");
    }
}
