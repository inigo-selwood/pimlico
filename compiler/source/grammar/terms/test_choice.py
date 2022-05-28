from __future__ import annotations

from grammar import terms
import text
from utilities import test


def test_parse_valid():

    # Simple choice
    choice = test.parse_valid('a | b', terms.Choice.parse)
    assert choice.__str__() == 'a | b'

    # Sequence within choice
    choice = test.parse_valid('(a b) | c', terms.Choice.parse)
    assert choice.__str__() == '(a b) | c'

    # Instanced choice within choice
    choice = test.parse_valid('(a | b)+ | c', terms.Choice.parse)
    assert choice.__str__() == '(a | b)+ | c'


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


def test_match():

    # Simple choice
    choice = test.parse_valid('\'a\' | \'b\'', terms.Choice.parse)

    # Check ('a', 'b') succeed
    match_text = test.match_valid(choice, 'a')
    assert match_text == 'a'
    match_text = test.match_valid(choice, 'b')
    assert match_text == 'b'

    # Check 'c' doesn't
    test.match_invalid(choice, 'c')

    # Precedence check
    # Make sure that the first successful match is returned
    choice = test.parse_valid('\'a\' | \'aa\'', terms.Choice.parse)
    match_text = test.match_valid(choice, 'aa')
    assert match_text == 'a'

