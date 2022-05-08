from grammar import Production

from .helpers import run_invalid_test, run_valid_test


def test_single():
    run_valid_test('term', Production.parse)


def test_simple():
    run_valid_test('term { // Some C++ }', Production.parse)


def test_compound():
    run_valid_test('(termA | termB) { // Some C++ }', Production.parse)
    run_valid_test('termA | termB { // Some C++ }', Production.parse)


def test_with_nesting_expression():
    run_valid_test('term { { // Some C++ } }', Production.parse)



def test_no_term():
    run_invalid_test(' ', Production.parse, [('expected a term', (1, 1))])


def test_unmatched_bracket():
    errors = [('expected \'}\', got end-of-file', (1, -1))]
    run_invalid_test('term {', Production.parse, errors)