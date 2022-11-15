
from pimlico.grammar.terms import Constant

from ...utilities import test

def test_constant_parse_valid():

    # Simple
    text = "'constant'"
    constant = test.run_parser(Constant.parse, text)
    assert isinstance(constant, Constant)
    assert constant.text == "constant"

    # Escape codes
    escape_codes = {
        "\'": "\'",
        "\\": "\\",
        "n": "\n",
        "r": "\r",
        "t": "\t",
    }
    codes = "".join([f"\\{code}" for code in escape_codes.keys()])
    text = f"'{codes}'"
    constant = test.run_parser(Constant.parse, text)
    assert constant.text == "".join(escape_codes.values())


def test_constant_parse_invalid():

    # Empty constant
    text = "''"
    errors = test.run_parser_fail(Constant.parse, text)
    excerpts = [
        ("empty constant", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Newline in constant
    text = "'\n"
    errors = test.run_parser_fail(Constant.parse, text)
    excerpts = [
        ("expected \"'\"", (1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # EOF in constant
    text = "'"
    errors = test.run_parser_fail(Constant.parse, text)
    excerpts = [
        ("expected \"'\"", (-1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)

    # Invalid escape code
    text = r"'\@'"
    errors = test.run_parser_fail(Constant.parse, text)
    excerpts = [
        ("invalid escape code", (1, 2)),
    ]
    assert test.error_present(errors, excerpts)

    # Incomplete escape code
    text = "'\\"
    errors = test.run_parser_fail(Constant.parse, text)
    excerpts = [
        ("incomplete escape code", (1, 2))
    ]
    for error in errors:
        print(error)
    assert test.error_present(errors, excerpts)


