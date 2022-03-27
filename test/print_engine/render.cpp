#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("print-engine.render") {

    SECTION("basic-token") {
        const std::string text = "@token";
        PrintEngine engine(text);

        engine.replace("token", "value");
        REQUIRE(engine.render() == "value");
    }

    SECTION("repeated-tokens") {
        const std::string text = "@token @token";
        PrintEngine engine(text);

        engine.replace("token", "value");
        REQUIRE(engine.render() == "value value");
    }

    SECTION("multiple-tokens") {
        const std::string text = "@token1 @token2";
        PrintEngine engine(text);

        engine.replace("token1", "value1");
        engine.replace("token2", "value2");
        REQUIRE(engine.render() == "value1 value2");
    }

    SECTION("token-mismatch") {
        const std::string text = "@token";
        PrintEngine engine(text);

        engine.replace("otherToken", "value");
        REQUIRE(engine.render() == text);
    }
}
