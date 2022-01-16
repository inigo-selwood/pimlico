#include <sstream>

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.error:print") {

    // Prints a message instance
    SECTION("message") {
        Buffer::Error errors;
        errors.add("message");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message");
    }

    // Prints a reference -- but the buffer is empty, so it should be treated
    // like a regular message
    SECTION("reference-invalid") {
        Buffer::Parse buffer("");
        Buffer::Error errors;
        errors.add("message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message");
    }

    // Prints a reference (valid, this time)
    // Should display the position (line 1, column 1), as well as the text on
    // that line ("hello world")
    SECTION("reference-valid") {
        Buffer::Parse buffer("hello world");
        Buffer::Error errors;
        errors.add("message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "[1:1] message\n    hello world");
    }

    // Print more than one error message
    SECTION("message-multiple") {
        Buffer::Error errors;
        errors.add("message-one");
        errors.add("message-two");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message-one\nmessage-two");
    }
}
