from text import ParseBuffer, ErrorBuffer

from grammar.terms import Reference


def test_reference():
    assert Reference.parse(ParseBuffer('a'), ErrorBuffer())
    assert Reference.parse(ParseBuffer('azAZ09_'), ErrorBuffer())
