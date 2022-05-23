from grammar import terms
from utilities import test


def test_parse_valid():

    sentences = [
        '__character__'
        '__identifier__'
        '__indent__'
        '__integer__'
        '__newline__'
        '__number__'
    ]

    for sentence in sentences:
        test.parse_valid(sentence, terms.Intrinsic.parse)
    