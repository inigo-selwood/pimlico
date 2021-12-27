#include <catch.hpp>

#include <grammar/term/terms/constant/constant.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.constant:parse") {

    SECTION("simple") {
        Buffer::Parse buffer("'hello'");
        Buffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant);
        REQUIRE(buffer.finished());

        delete constant;
    }

    SECTION("unexpected-end-of-file") {
        Buffer::Parse buffer("'hello");
        Buffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-character") {
        Buffer::Parse buffer("'\t'");
        Buffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("unexpected-newline") {
        Buffer::Parse buffer("'\n");
        Buffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("empty-constant") {
        Buffer::Parse buffer("''");
        Buffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }
}
