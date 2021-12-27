#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/range/range.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.range:print") {
    std::string text = "['a' - 'z']";

    Buffer::Parse buffer(text);
    Buffer::Error errors;

    Term *range = Range::parse(buffer, errors);

    std::stringstream stream;
    stream << *range;

    REQUIRE(stream.str() == text);

    delete range;
}
