#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

// Check that an empty buffer evaluates to false, and vice versa
TEST_CASE("buffer.error:true") {

    SECTION("empty") {
        Buffer::Error errors;
        errors.add("message");
        REQUIRE(errors);
    }

    SECTION("non-empty") {
        Buffer::Error errors;
        REQUIRE(errors == false);
    }
}
