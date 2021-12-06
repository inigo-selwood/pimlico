#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("create") {
    Buffer::Parse buffer("['a'-'z']");
    Buffer::Error errors;
    const Term *term = Term::parse(buffer, errors);
    REQUIRE(term);
}
