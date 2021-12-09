#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.create") {
    SECTION("empty") {
        Buffer::Parse buffer("");
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
    }
}

TEST_CASE("buffer.finished") {
    SECTION("empty") {
        Buffer::Parse buffer("");
        REQUIRE(buffer.finished());
    }

    SECTION("non-empty") {
        Buffer::Parse buffer(" ");
        REQUIRE(buffer.finished() == false);
    }
}

TEST_CASE("buffer.skip") {
    SECTION("space") {
        Buffer::Parse buffer(" \t\v\r");
        buffer.skip_space();
        REQUIRE(buffer.finished());
    }

    SECTION("line") {
        Buffer::Parse buffer(" \n");
        buffer.skip_line();
        REQUIRE(buffer.finished());
    }
}
