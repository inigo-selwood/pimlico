#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-whitespace") {
    Buffer::Parse buffer(" \t\v\r\n");
    buffer.skip_space();
    REQUIRE(buffer.finished());
}
