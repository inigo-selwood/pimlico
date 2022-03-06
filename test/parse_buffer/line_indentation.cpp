#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:line-indentation") {

    // Indentation for an empty buffer should be zero
    SECTION("empty") {
        ParseBuffer buffer("");
        REQUIRE(buffer.line_indentation() == 0);
    }

    // Spaces are treated as a unit of 1 indentation
    SECTION("single-space") {
        ParseBuffer buffer(" ");
        REQUIRE(buffer.line_indentation() == 1);
    }

    // Tabs are considered 4 units
    SECTION("tab") {
        ParseBuffer buffer("\t");
        REQUIRE(buffer.line_indentation() == 4);
    }

    // Indentation of a line not in the buffer is considered to be 0
    SECTION("out-of-range") {
        ParseBuffer buffer("\t");
        REQUIRE(buffer.line_indentation(2) == 0);
    }

    // Check the indentation of specific lines can be provided
    SECTION("specific-line") {
        ParseBuffer buffer("\n\t");
        REQUIRE(buffer.line_indentation(1) == 0);
        REQUIRE(buffer.line_indentation(2) == 4);
    }
}
