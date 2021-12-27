#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-line") {

    SECTION("single-newline") {
        Buffer::Parse buffer("\n");

        Buffer::Position end_position;
        end_position.index = 1;
        end_position.column = 1;
        end_position.line = 2;

        buffer.skip_line();
        REQUIRE(buffer.position == end_position);
    }

    SECTION("multiple-newline") {
        Buffer::Parse buffer("\n\n");

        Buffer::Position end_position;
        end_position.index = 1;
        end_position.column = 1;
        end_position.line = 2;

        buffer.skip_line();
        REQUIRE(buffer.position == end_position);
    }

    SECTION("non-empty-line") {
        Buffer::Parse buffer("hello world\n");
        buffer.skip_line();
        REQUIRE(buffer.finished());
    }

    SECTION("no-newline") {
        Buffer::Parse buffer("hello world");
        buffer.skip_line();
        REQUIRE(buffer.finished());
    }
}
