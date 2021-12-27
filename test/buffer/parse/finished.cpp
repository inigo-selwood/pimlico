#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:finished") {

    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.finished());
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.finished() == false);
    }
}
