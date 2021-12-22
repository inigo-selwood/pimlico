#include <iostream>

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("production.no-expression") {
    std::string text = "a";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    std::cerr << errors;
    REQUIRE(production);
    std::cout << text << " -> " << *production << '\n';

    delete production;
}

TEST_CASE("production.empty-expression") {
    std::string text = "a {}";
    Buffer::Parse buffer(text);
    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    std::cerr << errors;
    REQUIRE(production);
    std::cout << text << " -> " << *production << '\n';

    delete production;
}

TEST_CASE("production.non-empty-expression") {
    std::string text = "a {\n"
        "    return nullptr;\n"
        "}\n";
    Buffer::Parse buffer(text);

    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    std::cerr << errors;
    REQUIRE(production);
    std::cout << text << "\n->\n" << *production << '\n';

    delete production;
}

TEST_CASE("production.expression-with-binding") {
    std::string text = "value: a {\n"
        "    return @value;\n"
        "}\n";
    Buffer::Parse buffer(text);

    Buffer::Error errors;
    const Production *production = Production::parse(buffer, errors);

    std::cerr << errors;
    REQUIRE(production);
    std::cout << text << "\n->\n" << *production << '\n';

    delete production;
}
