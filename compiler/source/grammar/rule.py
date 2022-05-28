from __future__ import annotations

from copy import copy

import text
from text import helpers
import tools
import grammar


class Rule:

    def __init__(self, 
            name: str, 
            type: str, 
            productions: list, 
            position: text.Position):

        self.name = name
        self.type = type
        self.productions = productions
        self.position = position
        self.uses = []
        self.used_by = []
    
    def __str__(self):
        result = self.name

        if self.type:
            result += f' <{self.type}>'
        
        result += ' :='

        if len(self.productions) == 1:
            result += f' {self.productions[0].__str__()}'
        
        else:
            for production in self.productions:
                result += f'\n    - {production.__str__()}'
    
        return result
    
    @staticmethod
    def parse(buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Rule:
        
        postion = copy(buffer.position)
        name = helpers.parse_identifier(buffer)

        # Check for type
        buffer.skip_space()
        type = ''
        if buffer.match('<'):
            type_position = copy(buffer.position)

            type = helpers.parse_expression(buffer, ('<', '>'), errors)
            if type is None:
                return None
            type = type[1:-1]

            if not type:
                errors.add(__name__, 
                        'empty type', 
                        buffer.excerpt(type_position))
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
                        'expected productions', 
                        buffer.excerpt())
                return None
        
        return Rule(name, type, productions, postion)
    
    def link_rules(self, 
            rules: dict, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog):
        
        success = True

        for production in self.productions:
            link_success = production.link_rules(rules, buffer, errors, self)
            success = success and link_success
        
        return success