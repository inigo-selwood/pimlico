#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:line-indentation") {

    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.line_indentation() == 0);
    }

    SECTION("single-space") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.line_indentation() == 1);
    }

    SECTION("tab") {
        Buffer::Parse buffer("\t");
        REQUIRE(buffer.line_indentation() == 4);
    }

    SECTION("out-of-range") {
        Buffer::Parse buffer("\t");
        REQUIRE(buffer.line_indentation(2) == 0);
    }

    SECTION("specific-line") {
        Buffer::Parse buffer("\n\t");
        REQUIRE(buffer.line_indentation(1) == 0);
        REQUIRE(buffer.line_indentation(2) == 4);
    }
}
