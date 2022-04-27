from text import ParseBuffer, ErrorBuffer
from utilities import in_range

import grammar


class Term:

    domain = 'grammar.term'

    def __init__(self):
        pass

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer, root: bool = False):
        ''' Parses a term of any type

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a term
        errors: ErrorBuffer
            buffer for reporting errors
        root: bool
            whether to treat this term like a sequence (false by default)
        
        Returns
        -------
        reference: Reference
            the parsed term
        '''

        domain = f'{Term.domain}:parse'
        
        character = buffer.read()
        if root or character == '(':
            return grammar.terms.Sequence.parse(buffer, errors)
        if character == '\'':
            return grammar.terms.Constant.parse(buffer, errors)
        if character == '[':
            return grammar.terms.Range.parse(buffer, errors)
        if (character == '_'
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')
                or in_range(character, '0', '9')):
            return grammar.terms.Reference.parse(buffer, errors)
        elif character == '`':
            return grammar.terms.Set.parse(buffer, errors)
        
        else:
            errors.add(domain, 'expected a term', buffer.position)
            return None