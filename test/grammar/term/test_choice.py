from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Choice


class TestConstant:

    def test_normal(self):
        assert Choice.parse(ParseBuffer('a'), ErrorBuffer())
        assert Choice.parse(ParseBuffer('a | b'), ErrorBuffer())

    def test_normal(self):
        assert not Choice.parse(ParseBuffer('a |'), ErrorBuffer())
        assert not Choice.parse(ParseBuffer('a |\n'), ErrorBuffer())
        assert not Choice.parse(ParseBuffer('a ||'), ErrorBuffer())