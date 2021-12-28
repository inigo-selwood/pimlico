#include <sstream>

#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.rule:print") {
    std::string text = "rule <Type> :=\n"
        "    some_reference {\n"
        "        std::cout << \"something\";\n"
        "    }\n"
        "    'a_constant'\n";
    Buffer::Parse buffer(text);
    Buffer::Error errors;

    Rule *rule = Rule::parse(buffer, errors);

    std::stringstream stream;
    stream << *rule;

    REQUIRE(rule);
    REQUIRE(stream.str() == text);

    delete rule;
}
