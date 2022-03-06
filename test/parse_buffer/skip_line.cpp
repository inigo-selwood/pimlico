#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("buffer.parse:skip-line") {

    SECTION("single-newline") {
        ParseBuffer buffer("\n");

        ParseBuffer::Position end_position;
        end_position.index = 1;
        end_position.column = 1;
        end_position.line = 1;

        buffer.skip_line();
        REQUIRE(buffer.position == end_position);
    }

    SECTION("multiple-newline") {
        ParseBuffer buffer("\n\n");

        ParseBuffer::Position end_position;
        end_position.index = 1;
        end_position.column = 1;
        end_position.line = 2;

        buffer.skip_line();
        REQUIRE(buffer.position == end_position);
    }

    SECTION("non-empty-line") {
        ParseBuffer buffer("hello world\n");
        buffer.skip_line();
        REQUIRE(buffer.finished());
    }

    SECTION("no-newline") {
        ParseBuffer buffer("hello world");
        buffer.skip_line();
        REQUIRE(buffer.finished());
    }
}
