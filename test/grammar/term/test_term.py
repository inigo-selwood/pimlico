from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar import Term

class TestTerm:

    def test_normal(self):
        assert Term.parse(ParseBuffer('term'), ErrorBuffer(), True)
