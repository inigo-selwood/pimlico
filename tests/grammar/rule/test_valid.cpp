#include <iostream>
#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("rule.basic") {
    std::string text = "rule := a";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);

    if(errors)
        WARN(errors);
    REQUIRE(rule);

    INFO("text: " << text);
    INFO("result: " << *rule);
    REQUIRE(buffer.finished());

    delete rule;
}

TEST_CASE("rule.basic-typed-inline") {
    std::string text = "rule <std::string> := a";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);

    if(errors)
        WARN(errors);
    REQUIRE(rule);

    INFO("text: " << text);
    INFO("result: " << *rule);
    REQUIRE(buffer.finished());

    delete rule;
}

TEST_CASE("rule.basic-multi-line") {
    std::string text = "rule :=\n"
            "\ta\n"
            "\tb\n";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);

    if(errors)
        WARN(errors);
    REQUIRE(rule);

    INFO("text: " << text);
    INFO("result: " << *rule);
    REQUIRE(buffer.finished());

    delete rule;
}
