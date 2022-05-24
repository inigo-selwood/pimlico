from grammar import terms
from utilities import test


def test_parse_valid():
    
    # Big ol' seqeunce of terms
    text = 'a | b \'c\' [de] f `gh`'
    sequence = test.parse_valid(text, terms.Sequence.parse)
    assert sequence.__str__() == text

    # Instance bounds
    text = 'a+ b* c? d{2} e{2:} f{:2} g{2:3}'
    sequence = test.parse_valid(text, terms.Sequence.parse)
    assert sequence.__str__() == text

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

    # Error in child
    errors = test.parse_invalid('\'\'', terms.Sequence.parse)
