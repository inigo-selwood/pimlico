#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/reference/reference.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.reference:print") {
    std::string text = "this_is_a_reference";

    ParseBuffer buffer(text);
    ParseBuffer::Error errors;

    Term *reference = Reference::parse(buffer, errors);

    std::stringstream stream;
    stream << *reference;

    REQUIRE(stream.str() == text);

    delete reference;
}
