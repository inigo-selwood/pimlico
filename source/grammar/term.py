import copy
from copy import copy 

import utilities

# from grammar.terms import Choice, Constant, Range, Reference, Sequence, Set
import grammar

class Term:

    def __init__(self):
        self.bounds = (0, 0)

        self.predicate = None
        self.position = None
        self.binding = None
        self.type = ''

    @staticmethod
    def parse(buffer, errors, root=False):

        def parse_bound_value(buffer, errors):

            text = ''
            while True:

                if(buffer.finished()):
                    break

                character = buffer.peek()
                index = ord(character)
                if index >= ord('0') and index <= ord('9'):
                    text += buffer.read()
                else:
                    break

            if not text:
                return -1

            return f'{text}'

        def parse_binding(buffer):

            result = ''
            while True:
                if buffer.finished():
                    break

                character = buffer.peek()
                if utilities.is_identifier_letter(character):
                    result += buffer.read()
                else:
                    break

            return result

        def parse_specific_bounds(buffer, errors):

            def fail(message):
                errors.add('term.parse',
                        message,
                        buffer.position)
                return (0, 0)

            assert buffer.match('{', True)

            start_value = parse_bound_value(buffer, errors)

            buffer.skip_space()
            colon_present = False
            if buffer.match(':', True):
                colon_present = True

            buffer.skip_space()

            end_value = parse_bound_value(buffer, errors)

            if not buffer.match('}', True):
                return fail('expected \'}\'')

            # N instances
            if (start_value != -1
                    and end_value != -1
                    and colon_present == False):

                if start_value == 0:
                    return fail('zero-valued lower instance bound')

                return (start_value, start_value)

            # N or more instances
            elif (start_value != -1
                    and end_value != -1
                    and colon_present):

                if end_value == 0:
                    return fail('zero-valued upper instance bound')

                return (-1, end_value)

            # Between N, M values
            elif (start_value != -1
                    and end_value != -1
                    and colon_present):

                if end_value < start_value:
                    return fail('illogical instance bound')
                elif start_value == end_value and start_value == 0:
                    return fail('zero-valued instance bound')

                return (start_value, end_value)

            else:
                return fail('invalid instance bound')

        def parse_bounds(buffer, errors):

            if buffer.match('?', True):
                return (0, 1)
            elif buffer.match('*', True):
                return (0, -1)
            elif buffer.match('+', True):
                return (1, -1)
            elif buffer.match('{', True):
                return parse_specific_bounds(buffer, errors)
            else:
                return (1, 1)
        
        if root and not buffer.match('('):
            return grammar.terms.Sequence.parse(buffer, errors)

        predicate = ''
        if buffer.match('&', True):
            predicate = 'and'
        elif buffer.match('!', True):
            predicate = 'not'

        binding = ''
        character = buffer.peek()
        if utilities.is_identifier_letter(character):
            start_position = copy(buffer.position)
            identifier = parse_binding(buffer)

            buffer.skip_space()
            if identifier and buffer.match(':', True):
                binding = identifier
            else:
                buffer.position = start_position

        buffer.skip_space()
        term = None
        enclosed = buffer.match('(', True)
        if enclosed:
            term = grammar.terms.Sequence.parse(buffer, errors)
        else:
            character = buffer.peek()

            if character == '\'':
                term = grammar.terms.Constant.parse(buffer, errors)
            elif character == '[':
                term = grammar.terms.Range.parse(buffer, errors)
            elif character == '`':
                term = grammar.terms.Set.parse(buffer, errors)
            elif utilities.is_identifier_letter(character):
                term = grammar.terms.Reference.parse(buffer, errors)
            else:
                errors.add('term.parse',
                        'expected a term',
                        buffer.position)
                return None

        if not term:
            return None

        buffer.skip_space()
        if enclosed and not buffer.match(')'):
            errors.add('term.parse', 'expected \')\'', buffer.position)
            return None

        buffer.skip_space()
        term.bounds = parse_bounds(buffer, errors)
        if term.bounds == (0, 0):
            return None

        term.predicate = predicate
        term.binding = binding

        return term
