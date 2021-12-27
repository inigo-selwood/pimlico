#include <catch.hpp>

#include <grammar/term/terms/reference/reference.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.reference:parse") {

    Buffer::Parse buffer("some_reference");
    Buffer::Error errors;

    Term *reference = Reference::parse(buffer, errors);
    REQUIRE(reference);
    REQUIRE(buffer.finished());

    delete reference;
}
