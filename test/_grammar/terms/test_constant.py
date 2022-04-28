from grammar.terms import Constant

from ..helpers import run_invalid_test, run_valid_test


def test_constant():
    run_valid_test('\'a\'', Constant.parse)


def test_escaped_quote():
    run_valid_test('\'\\\'\'', Constant.parse)


def test_unexpected_end_of_file():
    errors = [('unexpected end-of-file', (1, -1))]
    run_invalid_test('\'', Constant.parse, errors)
    

def test_unexpected_end_of_line():
    errors = [('unexpected end-of-line', (1, -1))]
    run_invalid_test('\'\n', Constant.parse, errors)
