from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Reference


class TestReference:

    def test_normal(self):
        assert Reference.parse(ParseBuffer('someRuleName'), ErrorBuffer())

    def test_invalid(self):
        assert not Reference.parse(ParseBuffer('invalid$$'), ErrorBuffer())
