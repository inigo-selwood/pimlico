from grammar.terms import Constant

from ..helpers import run_invalid_parse_test, run_valid_parse_test


def test_constant():
    run_valid_parse_test('\'a\'', Constant.parse)


def test_escaped_quote():
    run_valid_parse_test('\'\\\'\'', Constant.parse)


def test_unexpected_end_of_file():
    errors = [('unexpected end-of-file', (1, -1))]
    run_invalid_parse_test('\'', Constant.parse, errors)
    

def test_unexpected_end_of_line():
    errors = [('unexpected newline', (1, -1))]
    run_invalid_parse_test('\'\n', Constant.parse, errors)
