from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Range


class TestRange:

    def test_normal(self):
        assert Range.parse(ParseBuffer('[a-z]'), ErrorBuffer())
        assert Range.parse(ParseBuffer('[ a - z ]'), ErrorBuffer())

    def test_missing_field(self):
        assert not Range.parse(ParseBuffer('[-z]'), ErrorBuffer())
        assert not Range.parse(ParseBuffer('[a-]'), ErrorBuffer())
        assert not Range.parse(ParseBuffer('[az]'), ErrorBuffer())
        assert not Range.parse(ParseBuffer('[a-z'), ErrorBuffer())

    def test_illogical_bounds(self):
        assert not Range.parse(ParseBuffer('[z-a]'), ErrorBuffer())

    def test_invalid_characters(self):
        assert not Range.parse(ParseBuffer('[\n-\r]'), ErrorBuffer())
