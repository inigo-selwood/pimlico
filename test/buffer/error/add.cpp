#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.error:add") {

    SECTION("message") {
        Buffer::Error errors;
        errors.add("message");
    }

    SECTION("default-reference") {
        Buffer::Parse buffer("");
        Buffer::Error errors;
        errors.add("message", buffer);
    }

    SECTION("reference") {
        Buffer::Parse buffer("");
        Buffer::Error errors;
        errors.add("message", buffer, buffer.position);
    }
}
