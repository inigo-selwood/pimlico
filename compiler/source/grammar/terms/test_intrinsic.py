from grammar import terms
from utilities import test


def test_parse_valid():

    sentences = [
        '__character__',
        '__identifier__',
        '__integer__',
        '__newline__',
        '__number__',
    ]

    for sentence in sentences:
        intrinsic = test.parse_valid(sentence, terms.Intrinsic.parse)
        assert intrinsic.__str__() == sentence

def test_parse_invalid():
    
    # Not an intrinsic value, cast to reference
    term = test.parse_valid('__invalid__', terms.Intrinsic.parse)
    assert term.type == 'reference'


def test_match():

    # Character
    intrinsic = test.parse_valid('__character__', terms.Intrinsic.parse)
    for ordinal in range(ord(' '), ord('~') + 1):
        character = chr(ordinal)
        match_text = test.match_valid(intrinsic, character)
        assert match_text == character

    # Identifier
    intrinsic = test.parse_valid('__identifier__', terms.Intrinsic.parse)
    match_text = test.match_valid(intrinsic, '_azAZ09')
    assert match_text == '_azAZ09'

    # Integer
    intrinsic = test.parse_valid('__integer__', terms.Intrinsic.parse)
    match_text = test.match_valid(intrinsic, '1024')
    assert match_text == '1024'
    match_text = test.match_valid(intrinsic, '0')
    assert match_text == '0'

    # Newline
    intrinsic = test.parse_valid('__newline__', terms.Intrinsic.parse)
    match_text = test.match_valid(intrinsic, '\n')
    assert match_text == '\n'

    # Number

    numbers = [
        '3.14', # Pi
        '-2.00', # G factor
        '6.02e23', # Avogadro
        '6.63e-34', # Planck
    ]

    intrinsic = test.parse_valid('__number__', terms.Intrinsic.parse)
    for number in numbers:

        match_text = test.match_valid(intrinsic, number)
        assert match_text == number