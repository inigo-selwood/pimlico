#include <iostream>
#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("basic-terms") {
    Buffer::Parse buffer("'a' b ['a' - 'z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}

TEST_CASE("choice-within-sequence") {
    Buffer::Parse buffer("'a' | 'b' 'c'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}

TEST_CASE("instance-hints") {
    Buffer::Parse buffer("'a'+ 'b'* 'c'? 'd'{1 : 1} 'e'{:1} 'f'{1:} 'g'{1}");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}

TEST_CASE("predicates") {
    Buffer::Parse buffer("&'a' !'b'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}

TEST_CASE("sequence-within-choice") {
    Buffer::Parse buffer("'a' | ('b' 'c')");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}

TEST_CASE("bindings") {
    Buffer::Parse buffer("a: 'a' b: b c: ['a' - 'z'] d: 'a' | 'b'");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);

    if(term == nullptr)
        std::cerr << errors;
    REQUIRE(term);
}
