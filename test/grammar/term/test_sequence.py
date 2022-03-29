from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Sequence


class TestConstant:

    def test_normal(self):
        assert Sequence.parse(ParseBuffer('a'), ErrorBuffer())
        assert Sequence.parse(ParseBuffer('a b'), ErrorBuffer())