from grammar import Program

from .helpers import run_invalid_test, run_valid_test


def test_simple():
    text = ('rule0 := term0'
            '\nrule1 := term1')
    run_valid_test(text, Program.parse)


def test_multi_line():
    text = ('rule0 :='
            '\n    - term0 {}'
            '\n    - term1 {}'
            '\n'
            '\nrule1 := term2 {}')
    run_valid_test(text, Program.parse)


def test_empty_text():
    run_invalid_test('', Program.parse, [('empty text', (-1, -1))])


def test_unexpected_indentation_increase():
    errors = [('unexpected indentation increase', (1, 5))]
    run_invalid_test('    rule := term', Program.parse, errors)


def test_expected_rule():
    run_invalid_test('{}', Program.parse, [('expected a rule', (1, 1))])


def test_duplicate_rule():
    text = ('rule := term'
            '\nrule := otherTerm')
    errors = [
        ('duplicate rule \'rule\'', (2, 1)),
        ('first declared here', (1, 1))
    ]
    run_invalid_test(text, Program.parse, errors)


def test_trailing_garbage():
    run_invalid_test('rule := term {} {}', 
            Program.parse, 
            [('expected a newline', (1, 17))])