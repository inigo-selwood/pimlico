#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-space") {
    ParseBuffer buffer(" \t\v\r");
    buffer.skip_space();
    REQUIRE(buffer.finished());
}
