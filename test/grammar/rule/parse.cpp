#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.rule:parse") {

    SECTION("basic") {
        Buffer::Parse buffer("rule := term");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("multiple-productions") {
        std::string text = "rule :=\n"
            "    'production 1'\n"
            "    'production 2'\n";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        INFO(errors);
        REQUIRE(rule);

        delete rule;
    }

    SECTION("typed-rule") {
        Buffer::Parse buffer("rule <Type> := term");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("nested-typed-rule") {
        Buffer::Parse buffer("rule <std::vector<Type>> := term");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("invalid-type") {
        Buffer::Parse buffer("rule <Type := term");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-assignment-symbol") {
        Buffer::Parse buffer("rule term");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-term") {
        Buffer::Parse buffer("rule := ~");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-productions") {
        Buffer::Parse buffer("rule :=");
        Buffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }
}
