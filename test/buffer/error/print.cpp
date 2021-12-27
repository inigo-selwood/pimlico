#include <sstream>

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.error:print") {

    SECTION("message") {
        Buffer::Error errors;
        errors.add("message");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message");
    }

    SECTION("reference-invalid") {
        Buffer::Parse buffer("");
        Buffer::Error errors;
        errors.add("message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message");
    }

    SECTION("reference-valid") {
        Buffer::Parse buffer("hello world");
        Buffer::Error errors;
        errors.add("message", buffer);

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "[1:1] message\n    hello world");
    }

    SECTION("message-multiple") {
        Buffer::Error errors;
        errors.add("message-one");
        errors.add("message-two");

        std::stringstream stream;
        stream << errors;

        REQUIRE(stream.str() == "message-one\nmessage-two");
    }
}