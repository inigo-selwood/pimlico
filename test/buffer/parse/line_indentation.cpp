#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:line-indentation") {

    // Indentation for an empty buffer should be zero
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.line_indentation() == 0);
    }

    // Spaces are treated as a unit of 1 indentation
    SECTION("single-space") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.line_indentation() == 1);
    }

    // Tabs are considered 4 units
    SECTION("tab") {
        Buffer::Parse buffer("\t");
        REQUIRE(buffer.line_indentation() == 4);
    }

    // Indentation of a line not in the buffer is considered to be 0
    SECTION("out-of-range") {
        Buffer::Parse buffer("\t");
        REQUIRE(buffer.line_indentation(2) == 0);
    }

    // Check the indentation of specific lines can be provided
    SECTION("specific-line") {
        Buffer::Parse buffer("\n\t");
        REQUIRE(buffer.line_indentation(1) == 0);
        REQUIRE(buffer.line_indentation(2) == 4);
    }
}
