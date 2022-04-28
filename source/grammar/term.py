from text import ParseBuffer, ErrorBuffer
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
                or in_range(character, 'A', 'Z')
                or in_range(character, '0', '9')):
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
        
        return term