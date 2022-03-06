#include <catch.hpp>

#include <grammar/term/terms/constant/constant.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.constant:parse") {

    SECTION("simple") {
        ParseBuffer buffer("'hello'");
        ParseBuffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant);
        REQUIRE(buffer.finished());

        delete constant;
    }

    SECTION("unexpected-end-of-file") {
        ParseBuffer buffer("'hello");
        ParseBuffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-character") {
        ParseBuffer buffer("'\t'");
        ParseBuffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("unexpected-newline") {
        ParseBuffer buffer("'\n");
        ParseBuffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }

    SECTION("empty-constant") {
        ParseBuffer buffer("''");
        ParseBuffer::Error errors;

        Term *constant = Constant::parse(buffer, errors);
        REQUIRE(constant == nullptr);
        REQUIRE(errors);
    }
}
