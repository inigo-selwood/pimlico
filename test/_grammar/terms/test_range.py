from grammar.terms import Range

from ..helpers import run_invalid_test, run_valid_test


def test_simple():
    grammars = [
        '[az]',
        '[09]',
        '[AZ]',
        '[ a z ]',
    ]

    for grammar in grammars:
        run_valid_test(grammar, Range.parse)


def test_escaped_bracket():
    run_valid_test(r'[[\]]', Range.parse)


def test_space():
    range = run_valid_test('[\' \'~]', Range.parse)
    assert range.values[0] == ' '


def test_end_of_file():
    grammars = [
        '[',
        '[a',
        '[az'
    ]

    for grammar in grammars:
        errors = [('unexpected end-of-file', (1, -1))]
        run_invalid_test(grammar, Range.parse, errors)


def test_end_of_line():
    grammars = [
        '[\n',
        '[a\n',
        '[az\n'
    ]

    for grammar in grammars:
        errors = [('unexpected end-of-line', (1, -1))]
        run_invalid_test(grammar, Range.parse, errors)


def test_too_few_characters():
    grammars = [
        '[]',
        '[a]'    
    ]

    for grammar in grammars:
        errors = [('too few characters', (1, 1))]
        run_invalid_test(grammar, Range.parse, errors)