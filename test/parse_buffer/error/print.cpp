#include <sstream>

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.error:print") {

    // Prints a message instance
    SECTION("message") {
        ParseBuffer::Error errors;
        errors.add("test.buffer.error", "message");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "(test.buffer.error) message");
    }

    // Prints a reference -- but the buffer is empty, so it should be treated
    // like a regular message
    SECTION("reference-invalid") {
        ParseBuffer buffer("");
        ParseBuffer::Error errors;
        errors.add("test.buffer.error", "message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "(test.buffer.error) message");
    }

    // Prints a reference (valid, this time)
    // Should display the position (line 1, column 1), as well as the text on
    // that line ("hello world")
    SECTION("reference-valid") {
        ParseBuffer buffer("hello world");
        ParseBuffer::Error errors;
        errors.add("test.buffer.error", "message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() ==
                "[1:1] (test.buffer.error) message\n"
                "    hello world");
    }

    // Print more than one error message
    SECTION("message-multiple") {
        ParseBuffer::Error errors;
        errors.add("test.buffer.error", "message-one");
        errors.add("test.buffer.error", "message-two");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() ==
                "(test.buffer.error) message-one\n"
                "(test.buffer.error) message-two");
    }
}
