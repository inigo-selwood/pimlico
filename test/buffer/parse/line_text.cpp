#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:line-text") {

    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.line_text() == "");
    }

    SECTION("single-space") {
        Buffer::Parse buffer("hello world");
        REQUIRE(buffer.line_text() == "hello world");
    }

    SECTION("out-of-range") {
        Buffer::Parse buffer("hello world");
        REQUIRE(buffer.line_text(2) == "");
    }

    SECTION("specific-line") {
        Buffer::Parse buffer("\nhello world");
        REQUIRE(buffer.line_text(1) == "");
        REQUIRE(buffer.line_text(2) == "hello world");
        REQUIRE(buffer.line_text(3) == "");
    }

    SECTION("specific-line") {
        Buffer::Parse buffer("hello world\n");
        REQUIRE(buffer.line_text(1) == "hello world");
        REQUIRE(buffer.line_text(2) == "");
    }
}
