from __future__ import annotations

from copy import copy

import text
from text import helpers
import tools
import grammar
from grammar import terms


class Production:

    def __init__(self, term: grammar.Term, expression: str):
        self.term = term
        self.expression = expression
    
    def __str__(self):
        return self.term.__str__()

    @staticmethod
    def parse(buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Production:
        
        # Check production is present
        if not grammar.Term.present(buffer):
            errors.add(__name__, 'expected a term', buffer.excerpt())
            return None
        
        # Parse the term
        term = terms.Sequence.parse(buffer, errors, True)
        if not term:
            return None
        
        # Check for, and parse an embedded expression
        expression = ''
        buffer.skip_space()
        expression_position = copy(buffer.position)
        if buffer.match('{{'):

            # Parse the expression, check it was okay
            expression = helpers.parse_expression(buffer, 
                    ('{{', '}}'), 
                    errors, 
                    permit_newlines=True)
            if expression is None:
                return None
            
            # Make sure the expression isn't empty
            expression = expression[2:-2].strip()
            if not expression:
                errors.add(__name__, 
                        'empty expression', 
                        buffer.excerpt(expression_position))
                return None
        
        return Production(term, expression)