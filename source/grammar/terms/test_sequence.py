from grammar import terms
from utilities import test


def test_parse_valid():
    
    # Big ol' seqeunce of terms
    test.parse_valid('a | b \'c\' [de] f `gh`', terms.Sequence.parse)

    # Check that (when root), brackets are consumed
    test.parse_valid('(a b) c', 
            terms.Sequence.parse, 
            arguments={'root': True})

    # Check reduction
    result = test.parse_valid('\'a\'', terms.Sequence.parse)
    assert result.type == 'constant'


def test_parse_invalid():

    # Expected closing bracket
    errors = test.parse_invalid('(a', terms.Sequence.parse)
    assert errors.has_value('expected \')\'', position=(1, -1))

    # Empty sequence
    errors = test.parse_invalid('( )', terms.Sequence.parse)
    assert errors.has_value('empty sequence', position=(1, 1))
