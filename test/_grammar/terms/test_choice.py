from grammar.terms import Choice

from ..helpers import run_invalid_test, run_valid_test


def test_simple():
    run_valid_test('a | b', Choice.parse)


def test_terms():
    run_valid_test('\'a\' | [bc] | d | (e f) | `gh`', Choice.parse)


def test_end_of_file():
    errors = [('unexpected end-of-file', (1, -1))]
    run_invalid_test('a |', Choice.parse, errors)


def test_end_of_line():
    errors = [('unexpected newline', (1, -1))]
    run_invalid_test('a |\n', Choice.parse, errors)


def test_empty_option():
    run_invalid_test('a || b', Choice.parse, [('empty option', (1, 3))])

    errors = [
        ('empty option', (1, 3)), 
        ('empty option', (1, 5))
    ]
    run_invalid_test('a | || b', Choice.parse, errors)


def test_duplicate_option():
    run_invalid_test('a | a', Choice.parse, [('duplicate option', (1, 5))])