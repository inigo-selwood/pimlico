from grammar import Rule

from .helpers import run_invalid_test, run_valid_test


def test_simple():
    run_valid_test('rule := term', Rule.parse)


def test_with_embed():
    run_valid_test('rule := term {}', Rule.parse)


def test_type():
    run_valid_test('rule <Type> := term', Rule.parse)


def test_with_nested_type():
    run_valid_test('rule <std::vector<std::string>> := term', Rule.parse)


def test_multi_line():
    text = ('rule := '
            '\n    - term0'
            '\n    - term1')
    run_valid_test(text, Rule.parse)


def test_multi_line_with_embeddings():
    text = ('rule := '
            '\n    - term0 {'
            '\n        // Some C++'
            '\n    }'
            '\n    - term1 {'
            '\n        // Some more C++'
            '\n    }')
    run_valid_test(text, Rule.parse)