from text import ParseBuffer, ErrorBuffer

from grammar import Term


def test_term():
    grammars = [
        '(a | b)',
        '\'a\'',
        '[az]',
        'a',
        '(a b)',
        '`ab`'
    ]

    for grammar in grammars:
        buffer = ParseBuffer(grammar)
        assert Term.parse(buffer, ErrorBuffer()), grammar
        assert buffer.finished(), grammar


def test_reduction():
    term = Term.parse(ParseBuffer('\'a\''), ErrorBuffer())
    assert term.type == 'constant'


def test_root():
    buffer = ParseBuffer('a | b \'c\' [de] f (g) `hj`')
    assert Term.parse(buffer, ErrorBuffer(), root=True)
    assert buffer.finished()


def test_not_root():
    buffer = ParseBuffer('a b')
    Term.parse(buffer, ErrorBuffer())
    assert not buffer.finished()