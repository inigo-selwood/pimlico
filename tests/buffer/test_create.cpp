#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("create") {
    SECTION("empty") {
        Buffer::Parse buffer("");
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
    }
}

TEST_CASE("finished") {
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.finished());
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.finished() == false);
    }
}
