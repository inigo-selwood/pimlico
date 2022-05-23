from __future__ import annotations

from grammar import terms
from utilities import test


def test_parse_valid():

    # Real simple ol' choice
    test.parse_valid('a | b', terms.Choice.parse)


def test_parse_invalid():

    # Unexpected EOF
    errors = test.parse_invalid('a |', terms.Choice.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    # Unexpected newline
    errors = test.parse_invalid('a |\n', terms.Choice.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))

    # Empty option
    errors = test.parse_invalid('a || b', terms.Choice.parse)
    assert errors.has_value('empty option', position=(1, 3))

    # Duplicate option
    errors = test.parse_invalid('a | a', terms.Choice.parse)
    assert errors.has_value('duplicate option', position=(1, 5))

    # Error in child
    errors = test.parse_invalid('\'\'', terms.Sequence.parse)
