#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.rule:parse") {

    SECTION("basic") {
        ParseBuffer buffer("rule := term");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("multiple-productions") {
        std::string text = "rule :=\n"
            "    'production 1'\n"
            "    'production 2'\n";
        ParseBuffer buffer(text);
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        INFO(errors);
        REQUIRE(rule);

        delete rule;
    }

    SECTION("typed-rule") {
        ParseBuffer buffer("rule <Type> := term");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("nested-typed-rule") {
        ParseBuffer buffer("rule <std::vector<Type>> := term");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule);
        REQUIRE(buffer.finished());

        delete rule;
    }

    SECTION("invalid-type") {
        ParseBuffer buffer("rule <Type := term");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-assignment-symbol") {
        ParseBuffer buffer("rule term");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-term") {
        ParseBuffer buffer("rule := ~");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }

    SECTION("no-productions") {
        ParseBuffer buffer("rule :=");
        ParseBuffer::Error errors;

        Rule *rule = Rule::parse(buffer, errors);
        REQUIRE(rule == nullptr);
        REQUIRE(errors);
    }
}
