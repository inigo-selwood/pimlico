from grammar import terms
from utilities import test


def test_parse_valid():
    
    # Simple range
    test.parse_valid('[az]', terms.Range.parse)

    # Range with some whitespace
    test.parse_valid('[ a z ]', terms.Range.parse)

    # Escaped closing bracket
    test.parse_valid(r'[[\]]', terms.Range.parse)

    # Escaped space
    test.parse_valid('[\' \'~]', terms.Range.parse)


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
    