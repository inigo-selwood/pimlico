#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.error:add") {

    // Adds instance of Message class
    SECTION("message") {
        ParseBuffer::Error errors;
        errors.add("test", "message");
    }

    // Adds instance of reference class, without specifying a buffer position
    // (ie: allows the `add` method to use current position of buffer)
    SECTION("default-reference") {
        ParseBuffer buffer("");
        ParseBuffer::Error errors;
        errors.add("test", "message", buffer);
    }

    // Adds instance of reference class, but specifies the position within
    // the buffer to be added
    SECTION("reference") {
        ParseBuffer buffer("");
        ParseBuffer::Error errors;
        errors.add("test", "message", buffer, buffer.position);
    }
}
