from perivale import Buffer

from pimlico.grammar.terms import Range

from ...utilities import test


def test_range_parse_valid():

    escape_codes = {
        "[az]": ("a", "z"),
        "[ a z ]": ("a", "z"),
        "[' '~]": (" ", "~"),
        r"[\]~]": ("]", "~"),
    }
    for text, bounds in escape_codes.items():
        constant = test.run_parser(Range.parse, text)
        assert constant.bounds == bounds
    

def test_range_parse_invalid():

    # Missing values
    text = "[ ]"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("expected range bound value", (1, 3)),
    ]
    assert test.error_present(errors, excerpts)

    # Missing second value
    text = "[' ' ]"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("expected range bound value", (1, 6)),
    ]
    assert test.error_present(errors, excerpts)

    # Newline in range
    text = "[' '~\n"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("expected ']'", (1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # EOF in range
    text = "[' '~"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("expected ']'", (-1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Illogical (equal) range
    text = "[aa]"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("illogical range bounds ('a' >= 'a')", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Illogical (less-than) range
    text = "[za]"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("illogical range bounds ('z' >= 'a')", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Incomplete escape code
    for text in ["[\\\n", "[\\"]:
        errors = test.run_parser_fail(Range.parse, text)
        excerpts = [
            ("incomplete escape code", (1, 2)),
        ]
        for error in errors:
            print(error)
        assert test.error_present(errors, excerpts)

    # Incomplete escape code
    text = "[\\n"
    errors = test.run_parser_fail(Range.parse, text)
    excerpts = [
        ("invalid escape code", (1, 2)),
    ]
    assert test.error_present(errors, excerpts)


def test_range_serialize():

    ranges = [
        "[az]",
        "[' '~]",
        "[[\]]",
    ]
    for text in ranges:
        term = Range.parse(Buffer(text), [])
        assert f"{term}" == text
    