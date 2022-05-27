from grammar import terms
from utilities import test


def test_parse_valid():
    
    # Simple range
    range = test.parse_valid('[az]', terms.Range.parse)
    assert range.__str__() == '[az]'

    # Range with some whitespace
    test.parse_valid('[ a z ]', terms.Range.parse)

    # Escaped closing bracket
    range = test.parse_valid(r'[[\]]', terms.Range.parse)
    assert range.__str__() == r'[[\]]'

    # Escaped space
    text = '[\' \'~]'
    range = test.parse_valid(text, terms.Range.parse)
    assert range.__str__() == text


def test_parse_invalid():

    # Unexpected EOF
    for sentence in ['[', '[a', '[az']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('unexpected end-of-file', position=(1, -1))

    # Unexpected newlines
    for sentence in ['[\n', '[a\n', '[az\n']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('unexpected newline', position=(1, -1))

    # Too few characters
    for sentence in ['[]', '[a]']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('too few characters', position=(1, 1))
    
    # Too few characters
    errors = test.parse_invalid('[za]', terms.Range.parse)
    assert errors.has_value('illogical (\'z\' > \'a\')', position=(1, 1))

    # Too few characters
    errors = test.parse_invalid('[aa]', terms.Range.parse)
    assert errors.has_value('redundant (constant)', position=(1, 1))
