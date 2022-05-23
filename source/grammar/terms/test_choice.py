from __future__ import annotations

from grammar import terms
from utilities import test


def test_parse_valid():
    # sentences = [
    #     'a | b',
    #     '\'a\' | [bc] | d | (e f) | `gh`',
    # ]

    test.parse_valid('a | b', terms.Choice.parse)


def test_parse_invalid():

    errors = test.parse_invalid('a |', terms.Choice.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    errors = test.parse_invalid('a |\n', terms.Choice.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))

    errors = test.parse_invalid('a || b', terms.Choice.parse)
    assert errors.has_value('empty option', position=(1, 3))

    errors = test.parse_invalid('a | a', terms.Choice.parse)
    assert errors.has_value('duplicate option', position=(1, 5))