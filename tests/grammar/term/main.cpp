#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("parse-basic-terms") {
    Buffer::Parse buffer("'a' b ['a' - 'z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}

TEST_CASE("parse-choice-within-sequence") {
    Buffer::Parse buffer("'a' | 'b' 'c'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}

TEST_CASE("instance-hints") {
    Buffer::Parse buffer("'a'+ 'b'* 'c'? 'd'{1 : 1} 'e'{:1} 'f'{1:} 'g'{1}");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}

TEST_CASE("predicates") {
    Buffer::Parse buffer("&'a' !'b'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}

TEST_CASE("sequence-within-choice") {
    Buffer::Parse buffer("'a' | ('b' 'c')");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}
