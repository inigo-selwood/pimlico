#include <iostream>

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("no-expression") {
    Buffer::Parse buffer("a");
    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    if(production == nullptr)
        std::cerr << errors;
    REQUIRE(production);
}

TEST_CASE("empty-expression") {
    Buffer::Parse buffer("a {}");
    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    if(production == nullptr)
        std::cerr << errors;
    REQUIRE(production);
}

TEST_CASE("non-empty-expression") {
    std::string production_string = "a {\n"
        "return nullptr;\n"
        "}\n";
    Buffer::Parse buffer(production_string);

    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    if(production == nullptr)
        std::cout << errors << std::endl;
    REQUIRE(production);
}

TEST_CASE("expression-with-binding") {
    std::string production_string = "value: a {\n"
        "return @value;\n"
        "}\n";
    Buffer::Parse buffer(production_string);

    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    if(production == nullptr)
        std::cerr << errors;
    REQUIRE(production);
}
