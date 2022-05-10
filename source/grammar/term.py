from copy import copy

from text import ParseBuffer, ErrorBuffer, parse_identifier
from utilities import in_range

import grammar


class Term:

    domain = 'grammar.term'

    def __init__(self):
        pass

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a term of any type

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        reference: Reference
            the parsed term
        '''

        domain = f'{Term.domain}:parse'

        binding = ''
        binding_position = copy(buffer.position)
        character = buffer.read()
        if (character == '_'
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')):
            binding = parse_identifier(buffer)

            buffer.skip_space()
            if not buffer.match(':', True):
                buffer.position = binding_position
                binding = ''

        buffer.skip_space()
        character = buffer.read()
        term = None
        if character == '(':
            term = grammar.terms.Sequence.parse(buffer, errors)
        elif character == '\'':
            term = grammar.terms.Constant.parse(buffer, errors)
        elif character == '[':
            term = grammar.terms.Range.parse(buffer, errors)
        elif (character == '_'
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')):
            term = grammar.terms.Reference.parse(buffer, errors)
        elif character == '`':
            term = grammar.terms.Set.parse(buffer, errors)

        else:
            errors.add(domain, 'expected a term', buffer.position)
            return None

        if not term:
            return None

        buffer.skip_space()
        if buffer.match('*', True):
            term.bounds = (0, -1)
        elif buffer.match('+', True):
            term.bounds = (1, -1)
        elif buffer.match('?', True):
            term.bounds = (0, 1)

        term.binding = binding
        return term

    @staticmethod
    def present(buffer: ParseBuffer) -> bool:
        ''' Checks for a term at the buffer's current position

        Arguments
        ---------
        buffer: ParseBuffer
            buffer in which to check for a term

        Returns
        -------
        term_present: bool
            true if there's a term at the buffer's current index which (could)
            be a term
        '''

        character = buffer.read()
        return (character == '\''
                or character == '['
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')
                or character == '('
                or character == '`')

    def link_references(self, rules: dict, errors: ErrorBuffer) -> bool:
        ''' Links references (redundant override)

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        success: bool
            whether or not the linking succeeded
        '''

        return True
