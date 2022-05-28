from grammar import terms
from utilities import test


def test_simple():
    
    # Simple
    set = test.parse_valid('`az`', terms.Set.parse)
    assert set.__str__() == '`az`'
    
    # Less simple
    set = test.parse_valid('`+-*/%`', terms.Set.parse)
    assert set.__str__() == '`+-*/%`'
    
    # Escaped closing backtick
    set = test.parse_valid(r'`.\``', terms.Set.parse)
    assert set.__str__() == r'`.\``'

    # Escape codes
    set = test.parse_valid('`\\t\\n`', terms.Set.parse)
    assert set.__str__() == '`\\t\\n`'


def test_empty_set():

    # Empty
    errors = test.parse_invalid('``', terms.Set.parse)
    assert errors.has_value('empty', position=(1, 1))

    # Duplicates
    errors = test.parse_invalid('`aa`', terms.Set.parse)
    assert errors.has_value('contains duplicates', position=(1, 1))

    # Could've been constant
    errors = test.parse_invalid('`a`', terms.Set.parse)
    assert errors.has_value('redundant (constant)', position=(1, 1))

    # Unexpected EOF
    errors = test.parse_invalid('`', terms.Set.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    # Unexpected newline
    errors = test.parse_invalid('`\n', terms.Set.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))


def test_match():

    # Basic set of arithmetic operators
    characters = '+-*/%'
    set = test.parse_valid(f'`{characters}`', terms.Set.parse)
    for character in characters:
        match_text = test.match_valid(set, character)
        assert match_text == character
    
    # Check all other characters fail
    for ordinal in range(ord(' '), ord('~') + 1):
        character = chr(ordinal)
        if character in set.values:
            continue
            
        test.match_invalid(set, character)