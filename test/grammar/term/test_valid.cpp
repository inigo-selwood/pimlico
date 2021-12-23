#include <iostream>
#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("term.basic-terms") {
    std::string text = "'a' b ['a' - 'z']";
    Buffer::Parse buffer("'a' b ['a' - 'z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}

TEST_CASE("term.choice-within-sequence") {
    std::string text = "'a' | 'b' 'c'";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}

TEST_CASE("term.instance-hints") {
    std::string text = "'a'+ 'b'* 'c'? 'd'{1 : 1} 'e'{:1} 'f'{1:} 'g'{1}";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}

TEST_CASE("term.predicates") {
    std::string text = "&'a' !'b'";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}

TEST_CASE("term.sequence-within-choice") {
    std::string text = "'a' | ('b' 'c')";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}

TEST_CASE("term.bindings") {
    std::string text = "a: 'a' b: b c: ['a' - 'z'] d: 'a' | 'b'";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors, true);

    if(errors)
        WARN(errors);
    REQUIRE(term);

    INFO("text: " << text);
    INFO("result: " << *term);
    REQUIRE(buffer.finished());

    delete term;
}
