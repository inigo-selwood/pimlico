#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.line-indentation") {
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.line_indentation() == 0);
    }

    SECTION("single-line") {
        Buffer::Parse buffer("\t");
        REQUIRE(buffer.line_indentation() == 4);
    }

    SECTION("multi-line") {
        std::string text = "\t\n"
                "\t\t\n";
        Buffer::Parse buffer(text);
        REQUIRE(buffer.line_indentation() == 4);
        REQUIRE(buffer.line_indentation(1) == 4);
        REQUIRE(buffer.line_indentation(2) == 8);
    }

    SECTION("whitespace-mixture") {
        Buffer::Parse buffer("\t  ");
        REQUIRE(buffer.line_indentation() == 6);
    }
}
