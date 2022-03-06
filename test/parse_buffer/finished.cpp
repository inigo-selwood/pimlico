#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

// Check an empty buffer is considered finished, and vice versa
TEST_CASE("buffer.parse:finished") {

    SECTION("empty") {
        ParseBuffer buffer("");
        REQUIRE(buffer.finished());
    }

    SECTION("non-empty") {
        ParseBuffer buffer(" ");
        REQUIRE(buffer.finished() == false);
    }
}
