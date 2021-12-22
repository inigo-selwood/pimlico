#include <iostream>
#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("rule.basic") {
    Buffer::Parse buffer("rule := a");
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);
    
    if(rule == nullptr) {
        std::cerr << errors;
        return;
    }
    std::cout << *rule << '\n';

    REQUIRE(rule);
    delete rule;
}

TEST_CASE("rule.basic-typed-inline") {
    Buffer::Parse buffer("rule <std::string> := a");
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);

    if(rule == nullptr) {
        std::cerr << errors;
        return;
    }
    std::cout << *rule << '\n';

    REQUIRE(rule);
    delete rule;
}

TEST_CASE("rule.basic-multi-line") {
    std::string program = "rule :=\n"
            "\ta\n"
            "\tb\n";
    Buffer::Parse buffer(program);
    Buffer::Error errors;
    const Rule *rule = Rule::parse(buffer, errors);

    if(rule == nullptr) {
        std::cerr << errors;
        return;
    }
    std::cout << *rule << '\n';

    REQUIRE(rule);
    delete rule;
}
