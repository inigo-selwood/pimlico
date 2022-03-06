#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/sequence/sequence.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.sequence:print") {

    SECTION("simple") {
        std::string text = "a b c";

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *sequence = Sequence::parse(buffer, errors);

        std::stringstream stream;
        stream << *sequence;

        REQUIRE(stream.str() == text);

        delete sequence;
    }

    SECTION("bounded") {
        std::string text = "(a b)+";

        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Term *sequence = Sequence::parse(buffer, errors);

        std::stringstream stream;
        stream << *sequence;

        REQUIRE(stream.str() == text);

        delete sequence;
    }
}
