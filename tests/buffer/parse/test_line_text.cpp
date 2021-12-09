#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("line-text") {
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.line_text() == "");
    }

    SECTION("single-line") {
        Buffer::Parse buffer("hello");
        REQUIRE(buffer.line_text() == "hello");
    }

    SECTION("multi-line") {
        std::string text = "hello\n"
                "world\n";
        Buffer::Parse buffer(text);
        REQUIRE(buffer.line_text() == "hello");
        REQUIRE(buffer.line_text(1) == "hello");
        REQUIRE(buffer.line_text(2) == "world");
    }

    SECTION("out-of-bounds") {
        Buffer::Parse buffer("hello world");
        REQUIRE(buffer.line_text(2) == "");
    }
}
