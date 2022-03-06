#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-whitespace") {
    ParseBuffer buffer(" \t\v\r\n");
    buffer.skip_whitespace();
    REQUIRE(buffer.finished());
}
