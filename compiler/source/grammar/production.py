from copy import copy
from utilities import format_c
from text import ParseBuffer, ErrorBuffer, parse_bounded_text
import grammar


class Production:
    
    domain = 'grammar.production'

    def __init__(self, term, expression: str):
        self.term = term
        self.expression = expression
    
    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parse a production expression

        Productions consist of a sequence of terms, and an optional expression:

        ```
        term0 term1 ... termN [{ // Expression }]
        ```

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at an expected production
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        production: Production
            the parsed production, or None if the parsing failed
        '''

        domain = f'{Production.domain}:parse'

        if not grammar.Term.present(buffer):
            errors.add(domain, 'expected a term', buffer.position)
            return None
        
        term = grammar.terms.Sequence.parse(buffer, errors, True)
        if not term:
            return None
        
        expression = ''
        buffer.skip_space()
        expression_position = copy(buffer.position)
        if buffer.match('{'):

            expression = parse_bounded_text(buffer, errors, '{', '}').strip()
            if not expression:
                errors.add(domain, 'empty expression', expression_position)
                return None
        
            expression = format_c(expression)
        
        return Production(term, expression)
    
    def link_references(self, rules: dict, errors: ErrorBuffer) -> bool:
        ''' Links rules to references in this production's terms

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

        return self.term.link_references(rules, errors)