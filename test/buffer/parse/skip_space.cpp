#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-space") {
    Buffer::Parse buffer(" \t\v\r");
    buffer.skip_space();
    REQUIRE(buffer.finished());
}
