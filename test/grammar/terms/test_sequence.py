from text import ParseBuffer, ErrorBuffer

from grammar.terms import Sequence


def test_sequence():
    assert Sequence.parse(ParseBuffer('(a)'), ErrorBuffer())

    buffer = ParseBuffer('(a | b \'c\' [de] f (g) `hi`)')
    assert Sequence.parse(buffer, ErrorBuffer())
    assert buffer.finished()


def test_reduction():
    term = Sequence.parse(ParseBuffer('\'a\''), ErrorBuffer())
    assert term.type == 'constant'