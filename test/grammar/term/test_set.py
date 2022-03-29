from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Set


class TestSet:

    def test_normal(self):
        assert Set.parse(ParseBuffer('`abc`'), ErrorBuffer())

    def test_escaped_backtick(self):
        assert Set.parse(ParseBuffer('`\\``'), ErrorBuffer())

    def test_empty(self):
        assert not Set.parse(ParseBuffer('``'), ErrorBuffer())

    def test_interrupted(self):
        assert not Set.parse(ParseBuffer('`'), ErrorBuffer())
        assert not Set.parse(ParseBuffer('`\n'), ErrorBuffer())
