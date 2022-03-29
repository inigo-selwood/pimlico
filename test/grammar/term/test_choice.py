from parse_buffer import ParseBuffer
from error_buffer import ErrorBuffer

from grammar.terms import Choice


class TestConstant:

    def test_reference_reduce(self):
        term = Choice.parse(ParseBuffer('a'), ErrorBuffer())

        assert term
        assert term.type == 'reference'

    def test_choice_standard(self):
        choice = Choice.parse(ParseBuffer('a | b'), ErrorBuffer())

        assert choice 
        assert choice.type == 'choice'
        assert len(choice.values) == 2
        
        for value in choice.values:
            assert value.type == 'reference'

    def test_malformed(self):

        tests = {
            'a |': ('choice.parse', 'unexpected end-of-file', (4, 1)),
            'a | \n': ('choice.parse', 'unexpected end-of-line', (5, 1)),
        }

        for grammar, fields in tests.items():
            trueSection, trueMessage, truePosition = fields

            buffer = ParseBuffer(grammar)
            errors = ErrorBuffer()
            choice = Choice.parse(buffer, errors)
            assert not choice 
            assert len(errors.instances) == 1
            
            section, message, position = errors.instances[0]
            assert section == trueSection
            assert message == trueMessage
            assert (position.column, position.line) == truePosition