#include <sstream>

#include <catch.hpp>

#include <grammar/term/terms/constant/constant.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term.constant:print") {
    std::string text = "'hello'";

    Buffer::Parse buffer(text);
    Buffer::Error errors;

    Term *constant = Constant::parse(buffer, errors);

    std::stringstream stream;
    stream << *constant;

    REQUIRE(stream.str() == text);

    delete constant;
}
