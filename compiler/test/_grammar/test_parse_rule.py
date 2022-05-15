from grammar import Rule

from .helpers import run_invalid_parse_test, run_valid_parse_test


def test_simple():
    run_valid_parse_test('rule := term', Rule.parse)


def test_with_embed():
    run_valid_parse_test('rule := term { // Some C++ }', Rule.parse)


def test_type():
    run_valid_parse_test('rule <Type> := term', Rule.parse)


def test_with_nested_type():
    run_valid_parse_test('rule <std::vector<std::string>> := term', Rule.parse)


def test_multi_line():
    text = ('rule := '
            '\n    - term0'
            '\n    - term1')
    run_valid_parse_test(text, Rule.parse)


def test_multi_line_with_embeddings():
    text = ('rule := '
            '\n    - term0 {'
            '\n        // Some C++'
            '\n    }'
            '\n    - term1 {'
            '\n        // Some more C++'
            '\n    }')
    run_valid_parse_test(text, Rule.parse)


def test_expected_assign():
    text = 'rule # Missing \':=\''
    errors = [('expected \':=\'', (1, -1))]
    run_invalid_parse_test(text, Rule.parse, errors)


def test_expected_productions():
    errors = [('expected one or more productions', (1, 9))]
    run_invalid_parse_test('rule := {}', Rule.parse, errors)


def test_expected_term():
    text = ('rule :='
            '\n    - someTerm { // Some C++ }'
            '\n    - { // Some more C++}')
    errors = [('expected a term', (3, 7))]
    run_invalid_parse_test(text, Rule.parse, errors)


def test_unexpected_indentation_increase():
    text = ('rule :='
            '\n    - term0'
            '\n        - term1')
    errors = [('invalid indentation', (3, 10))]
    run_invalid_parse_test(text, Rule.parse, errors)


def test_unmatched_bracket():
    errors = [('expected \'>\', got end-of-file', (1, -1))]
    run_invalid_parse_test('rule <', Rule.parse, errors)