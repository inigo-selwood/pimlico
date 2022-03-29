from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar import Term

class TestTerm:

    def test_normal(self):
        assert Term.parse(ParseBuffer('term'), ErrorBuffer(), True)
        assert Term.parse(ParseBuffer('(term)'), ErrorBuffer(), True)
        assert Term.parse(ParseBuffer('(a | b) \'c\' [a-z] term `+-*/`'), ErrorBuffer(), True)
    
    def test_enclosed(self):
        assert not Term.parse(ParseBuffer('(term'), ErrorBuffer(), True)