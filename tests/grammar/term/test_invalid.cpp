#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("empty-constant") {
    Buffer::Parse buffer("''");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term == nullptr);
}

TEST_CASE("empty-sequence") {
    Buffer::Parse buffer("()");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term == nullptr);
}

TEST_CASE("illogical-instance-bounds") {
    SECTION("start-greater-than-end") {
        Buffer::Parse buffer("'a'{1 : 0}");
        Buffer::Error errors;
        const Term *term = Term::parse(buffer, errors);
        REQUIRE(term == nullptr);
    }


    SECTION("end-is-zero") {
        Buffer::Parse buffer("'a'{0 : 0}");
        Buffer::Error errors;
        const Term *term = Term::parse(buffer, errors);
        REQUIRE(term == nullptr);
    }
}

TEST_CASE("illogical-range") {
    Buffer::Parse buffer("['z' - 'a']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term == nullptr);
}

TEST_CASE("multiple-character-range") {
    Buffer::Parse buffer("['AB' - 'Z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term == nullptr);
}

TEST_CASE("unclosed-constant") {
    Buffer::Parse buffer("'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term == nullptr);
}
