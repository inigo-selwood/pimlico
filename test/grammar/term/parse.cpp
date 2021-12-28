#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term:parse") {

    SECTION("basic-non-root") {
        std::string text = GENERATE(as<std::string>{},
                "a | b",
                "'constant'",
                "['a' - 'z']",
                "reference",
                "a b");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("simple-enclosed") {
        Buffer::Parse buffer("(value)");
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("invalid-enclosed") {
        Buffer::Parse buffer("(value");
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors);
        REQUIRE(term == nullptr);
        REQUIRE(errors);
    }

    SECTION("invalid-term") {
        Buffer::Parse buffer("[a-z]");
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term == nullptr);
        REQUIRE(errors);
    }

    SECTION("simple-binding") {
        Buffer::Parse buffer("binding: value");
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("simple-predicated") {
        std::string text = GENERATE(as<std::string>{}, "&term", "!term");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("simple-bounds") {
        std::string text = GENERATE(as<std::string>{},
                "term+",
                "term*",
                "term?");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("term-with-expression") {
        std::string text = "term {\n"
                "    std::cout << \"hello world\\n\";"
                "}";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);

        delete term;
    }

    SECTION("specific-bounds-valid") {
        std::string text = GENERATE(as<std::string>{},
                "term{1}",
                "term{1:}",
                "term{:1}",
                "term{1:2}");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term);
        REQUIRE(buffer.finished());

        delete term;
    }

    SECTION("specific-bounds-invalid") {
        std::string text = GENERATE(as<std::string>{},
                "term{:}",
                "term{1 2}",
                "term{0");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        REQUIRE(term == nullptr);
        REQUIRE(errors);
    }

    SECTION("specific-bounds-illogical") {
        std::string text = GENERATE(as<std::string>{},
                "term{0}",
                "term{:0}",
                "term{0:0}",
                "term{2:1}");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);
        INFO(text)
        REQUIRE(term == nullptr);
        REQUIRE(errors);
    }
}
