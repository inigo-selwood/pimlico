#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.position") {
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.position.column == 0);
        REQUIRE(buffer.position.line == 0);
        REQUIRE(buffer.position.index == 0);
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.position.column == 1);
        REQUIRE(buffer.position.line == 1);
        REQUIRE(buffer.position.index == 0);
    }
}
