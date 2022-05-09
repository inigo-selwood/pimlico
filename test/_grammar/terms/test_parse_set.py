from grammar.terms import Set

from ..helpers import run_invalid_parse_test, run_valid_parse_test


def test_simple():
    run_valid_parse_test('`az`', Set.parse)


def test_symbols():
    run_valid_parse_test('`+-*/%`', Set.parse)


def test_escaped_backtick():
    run_valid_parse_test(r'`.\``', Set.parse)


def test_escape_codes():
    codes = {
        '\\t': '\t',
        '\\n': '\n'
    }

    for code, value in codes.items():
        result = run_valid_parse_test(f'`.\\{code}`', Set.parse)
        assert value in result.values


def test_empty_set():
    run_invalid_parse_test('``', Set.parse, [('empty', (1, 1))])


def test_redundant_constant():
    run_invalid_parse_test('`a`', 
            Set.parse, 
            [('redundant (constant)', (1, 1))])


def test_redundant_range():
    run_invalid_parse_test('`abc`', Set.parse, [('redundant (range)', (1, 1))])


def test_end_of_line():
    errors = [('unexpected newline', (1, -1))]
    run_invalid_parse_test('`\n', Set.parse, errors)


def test_end_of_file():
    run_invalid_parse_test('`', 
            Set.parse,
            [('unexpected end-of-file', (1, -1))])