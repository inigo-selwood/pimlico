#include <sstream>

#include <catch.hpp>

#include <pimlico.hpp>

using namespace Pimlico;

TEST_CASE("grammar.term:print") {

    SECTION("simple-binding") {
        std::string text = "binding: term";
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);

        std::stringstream stream;
        stream << *term;

        REQUIRE(term);
        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

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

        std::stringstream stream;
        stream << *term;

        REQUIRE(term);
        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete term;
    }

    SECTION("complex-bounds") {
        std::string text = GENERATE(as<std::string>{},
                "term{2}",
                "term{2:}",
                "term{:2}",
                "term{1:2}");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);

        std::stringstream stream;
        stream << *term;

        REQUIRE(term);
        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete term;
    }

    SECTION("predicates") {
        std::string text = GENERATE(as<std::string>{}, "&term", "!term");
        Buffer::Parse buffer(text);
        Buffer::Error errors;

        Term *term = Term::parse(buffer, errors, true);

        std::stringstream stream;
        stream << *term;

        REQUIRE(term);
        REQUIRE(buffer.finished());
        REQUIRE(stream.str() == text);

        delete term;
    }
}
