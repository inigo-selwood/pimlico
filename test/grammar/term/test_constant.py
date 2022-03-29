from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Constant


class TestConstant:

    def test_normal(self):
        assert Constant.parse(ParseBuffer('\'test\''), ErrorBuffer())

    def test_empty(self):
        assert not Constant.parse(ParseBuffer('\'\''), ErrorBuffer())

    def test_terminated(self):
        assert not Constant.parse(ParseBuffer('\''), ErrorBuffer())
        assert not Constant.parse(ParseBuffer('\'\n'), ErrorBuffer())
