#include <catch.hpp>
#include <pimlico.hpp>

using namespace Pimlico;

// Check an empty buffer is considered finished, and vice versa
TEST_CASE("buffer.print.form:serialize") {

    SECTION("single") {
        Buffer::Form form;
        form.build("@token");
        form.substitute("token", "value");
        REQUIRE(form.serialize() == "value");
    }

    SECTION("multiple") {
        Buffer::Form form;
        form.build("@token_one @token_two");

        form.substitute("token_one", "one");
        form.substitute("token_two", "two");

        REQUIRE(form.serialize() == "one two");
    }

    SECTION("repeated") {
        Buffer::Form form;
        form.build("@token @token");
        form.substitute("token", "value");
        REQUIRE(form.serialize() == "value value");
    }

    SECTION("typical") {
        Buffer::Form form;
        const std::string templateText =
                "if(character < @lower || character > @upper)\n"
                "    break;\n";
        form.build(templateText);
        form.substitute("lower", "'a'");
        form.substitute("upper", "'z'");

        const std::string resultText =
                "if(character < 'a' || character > 'z')\n"
                "    break;\n";
        REQUIRE(form.serialize() == resultText);
    }
}
