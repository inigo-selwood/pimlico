from grammar import terms
from utilities import test


def test_parse_valid():

    sentences = [
        '__character__',
        '__identifier__',
        '__indent__',
        '__integer__',
        '__newline__',
        '__number__',
    ]

    for sentence in sentences:
        intrinsic = test.parse_valid(sentence, terms.Intrinsic.parse)
        assert intrinsic.__str__() == sentence
    
    # Not an intrinsic value, cast to reference
    term = test.parse_valid('__invalid__', terms.Intrinsic.parse)
    assert term.type == 'reference'