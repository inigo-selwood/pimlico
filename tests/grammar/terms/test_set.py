from pimlico.grammar.terms import Set

from ...utilities import test


def test_set_parse_valid():

    # Simple
    text = "`+-*/%`"
    set = test.run_parser(Set.parse, text)
    assert set.values == "+-*/%"

    # Escape codes
    escape_codes = {
        "n": "\n",
        "r": "\r",
        "t": "\t",
    }
    codes = "".join([f"\\{code}" for code in escape_codes.keys()])
    text = f"`{codes}`"
    set = test.run_parser(Set.parse, text)
    assert set.values == "".join(escape_codes.values())


def test_constant_parse_invalid():

    # Empty set
    text = "``"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("empty set", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Newline in set
    text = "`\n"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("expected '`'", (1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # EOF in set
    text = "`"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("expected '`'", (-1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Invalid escape code
    text = r"`\@`"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("invalid escape code", (1, 2)),
    ]
    assert test.error_present(errors, excerpts)

    # Incomplete escape code
    text = "`\\"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("incomplete escape code", (1, 2))
    ]
    assert test.error_present(errors, excerpts)

    # Duplicate value
    text = "`aa`"
    errors = test.run_parser_fail(Set.parse, text)
    excerpts = [
        ("duplicate value in set", (1, 3))
    ]
    assert test.error_present(errors, excerpts)

    




