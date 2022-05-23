from __future__ import annotations

from copy import copy

import text
from text import helpers
import tools
import grammar


class Rule:

    def __init__(self, name: str, productions: list):
        self.name = name
        self.productions = productions
    
    def parse(self, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Rule:
        
        start_position = copy(buffer.position)
        name = helpers.parse_identifier(buffer)

        # Check for type
        buffer.skip_space()
        type = ''
        if buffer.match('<'):
            type = helpers.parse_expression(buffer, ('<', '>'), errors)
            if type is None:
                return None

        # Check for assign symbol
        buffer.skip_space()
        if not buffer.match(':=', consume=True):
            errors.add(__name__, 'expected \':=\'', buffer.excerpt())
            return None
        
        # Parse inline production
        productions = []
        buffer.skip_space()
        if grammar.Term.present(buffer):

            production = grammar.Production.parse(buffer, errors)
            if not production:
                return None
            productions.append(production)
        
        # Parse multi-choice productions
        else:
            
            while True:

                # Check for a dash
                position = copy(buffer.position)
                buffer.skip_space(include_newlines=True)
                if not buffer.match('-'):
                    buffer.position = position
                    break

                # Skip dash
                position = copy(buffer.position)
                buffer.increment()

                # Check indentation
                if buffer.line_indentation() != 4:
                    errors.add(__name__, 
                            'invalid indentation', 
                            buffer.excerpt(position))
                    return None
                
                # Check there's a term present
                buffer.skip_space()
                if not grammar.Term.present(buffer):
                    errors.add(__name__, 
                            'expected a term', 
                            buffer.excerpt())
                    return None
                
                # Parse the production
                production = grammar.Production.parse(buffer, errors)
                if not production:
                    return None
                productions.append(production)
            
            if not productions:
                errors.add(__name__, 
                        'expected one or more productions', 
                        buffer.excerpt())
                return None
        
        return Rule(name, type, productions, start_position)