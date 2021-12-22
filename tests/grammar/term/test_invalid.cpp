#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("term.empty-constant") {
    Buffer::Parse buffer("''");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);
    REQUIRE(term == nullptr);
}

TEST_CASE("term.empty-sequence") {
    Buffer::Parse buffer("()");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);
    REQUIRE(term == nullptr);
}

TEST_CASE("term.illogical-instance-bounds") {
    SECTION("start-greater-than-end") {
        Buffer::Parse buffer("'a'{1 : 0}");
        Buffer::Error errors;
        const Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term == nullptr);
    }


    SECTION("end-is-zero") {
        Buffer::Parse buffer("'a'{0 : 0}");
        Buffer::Error errors;
        const Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term == nullptr);
    }


    SECTION("zero-instance") {
        Buffer::Parse buffer("'a'{0}");
        Buffer::Error errors;
        const Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term == nullptr);
    }
}

TEST_CASE("term.illogical-range") {
    Buffer::Parse buffer("['z' - 'a']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);
    REQUIRE(term == nullptr);
}

TEST_CASE("term.multiple-character-range") {
    Buffer::Parse buffer("['AB' - 'Z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);
    REQUIRE(term == nullptr);
}

TEST_CASE("term.unclosed-constant") {
    Buffer::Parse buffer("'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);
    REQUIRE(term == nullptr);
}
