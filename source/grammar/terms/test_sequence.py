from grammar import terms
from utilities import test


def test_parse_valid():
    test.parse_valid('a | b \'c\' [de] f `gh`', terms.Sequence.parse)

    test.parse_valid('(a b) c', 
            terms.Sequence.parse, 
            arguments={'root': True})

    result = test.parse_valid('\'a\'', terms.Sequence.parse)
    assert result.type == 'constant'


def test_parse_invalid():

    errors = test.parse_invalid('(a', terms.Sequence.parse)
    assert errors.has_value('expected \')\'', position=(1, -1))

    errors = test.parse_invalid('( )', terms.Sequence.parse)
    assert errors.has_value('empty sequence', position=(1, 1))
