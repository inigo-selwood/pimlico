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


def test_instance_bounds():
    grammars = {
        'a+': (1, -1),
        'a*': (0, -1),
        'a?': (0, 1)
    }

    for grammar, bounds in grammars.items():
        buffer = ParseBuffer(grammar)
        term = Term.parse(buffer, ErrorBuffer())
        assert term
        assert term.bounds == bounds, grammar
        assert buffer.finished(), grammar


def test_reduction():
    term = Term.parse(ParseBuffer('\'a\''), ErrorBuffer())
    assert term.type == 'constant'