from grammar.terms import Sequence

from ..helpers import run_invalid_parse_test, run_valid_parse_test


def test_sequence():
    run_valid_parse_test('a | b \'c\' [de] f `gh`', Sequence.parse)


def test_root_sequence():
    run_valid_parse_test('(a b) c', Sequence.parse, arguments={'root': True})


def test_unclosed():
    run_invalid_parse_test('(a', Sequence.parse, [('expected \')\'', (1, -1))])


def test_empty():
    run_invalid_parse_test('( )', Sequence.parse, [('empty sequence', (1, 1))])


def test_reduction():
    result = run_valid_parse_test('\'a\'', Sequence.parse)
    assert result.type == 'constant'