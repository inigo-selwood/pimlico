from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Constant


class TestConstant:

    def test_normal(self):
        buffer = ParseBuffer('\'test\'')
        constant = Constant.parse(buffer, ErrorBuffer())
        assert constant 
        assert constant.type == 'constant'
        assert constant.value == 'test'

    def test_empty(self):
        assert not Constant.parse(ParseBuffer('\'\''), ErrorBuffer())

    def test_terminated(self):
        assert not Constant.parse(ParseBuffer('\''), ErrorBuffer())
        assert not Constant.parse(ParseBuffer('\'\n'), ErrorBuffer())