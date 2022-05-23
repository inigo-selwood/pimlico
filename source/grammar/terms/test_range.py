from grammar import terms
from utilities import test


def test_parse_valid():
    test.parse_valid('[az]', terms.Range.parse)

    test.parse_valid('[ a z ]', terms.Range.parse)

    test.parse_valid(r'[[\]]', terms.Range.parse)

    test.parse_valid('[\' \'~]', terms.Range.parse)


def test_parse_invalid():

    for sentence in ['[', '[a', '[az']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('unexpected end-of-file', position=(1, -1))

    for sentence in ['[\n', '[a\n', '[az\n']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('unexpected newline', position=(1, -1))

    for sentence in ['[]', '[a]']:
        errors = test.parse_invalid(sentence, terms.Range.parse)
        assert errors.has_value('too few characters', position=(1, 1))
    