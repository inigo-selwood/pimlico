#include <catch.hpp>

#include <grammar/term/terms/sequence/sequence.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.sequence:parse") {

    SECTION("simple") {
        ParseBuffer buffer("a b c");
        ParseBuffer::Error errors;

        Term *sequence = Sequence::parse(buffer, errors);
        REQUIRE(sequence);
        REQUIRE(buffer.finished());

        delete sequence;
    }
}
