from copy import copy

from utilities import indent_text
from text import (Position,
        ParseBuffer,
        ErrorBuffer,
        parse_bounded_text,
        parse_identifier)
import grammar


class Rule:

    domain = 'grammar.rule'

    def __init__(self, 
            name: str, 
            type: str, 
            productions: list, 
            position: Position):
        
        self.name = name
        self.type = type
        self.productions = productions
        self.position = position
    
    def __str__(self):
        result = self.name
        
        if self.type:
            result += f' <{self.type}>'
        
        result += ' :='

        # Inline productions get printed on the same line
        if len(self.productions) == 1:
            result += f' {self.productions[0].__str__()}'
        
        # Multiple productions get written on a new line with a bullet-point
        else:
            for production in self.productions:
                text = f'\n    - {production.__str__()}'
                result = indent_text(text, 1)
        
        return result

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a rule

        There are two forms of rules. In the first case, there's just one 
        inline value:

        ```
        rule <type> := term {}
        ```

        When you want a rule to have multiple productions, they're indented by
        1 tab, and prefixed with a hyphen:

        ```
        rule <type> :=
            - term1 {}
            - term2 {}
        ```

        Arguments
        ---------
        buffer: ParseBuffer
            the buffer where the rule is present
        errors: ErrorBuffer
            for reporting errors
        
        Returns
        -------
        rule: Rule
            the parsed rule
        '''

        domain = f'{Rule.domain}:parse'
        start_position = copy(buffer.position)

        name = parse_identifier(buffer)
        assert name, 'expected a name'

        buffer.skip_space()
        type = ''
        if buffer.match('<'):
            type = parse_bounded_text(buffer, errors, '<', '>', permit_newlines=False)
            if not type:
                return None

        buffer.skip_space()
        if not buffer.match(':=', True):
            errors.add(domain, 'expected \':=\'', buffer.position)
            return None
        
        # There are 2 types of rule forms; one has a single production on the 
        # same line, and the other has a list of them on newlines, indented by
        # 1 tab, and prefixed with a hyphen `-`.
        # This should be the inline case
        productions = []
        buffer.skip_space()
        if grammar.Term.present(buffer):
            production = grammar.Production.parse(buffer, errors)
            if not production:
                return None
            productions.append(production)
        
        # If there isn't an inline term, we presume there's multiple
        # bullet-point indented ones
        else:
            
            while True:
                pre_production_position = copy(buffer.position)
                buffer.skip_space(include_newlines=True)
                if not buffer.match('-', True):
                    buffer.position = pre_production_position
                    break

                # Make sure productions are indented properly
                elif buffer.line_indentation() != 4:
                    errors.add(domain, 
                            'invalid indentation', 
                            buffer.position)
                    return None
                
                # Check there's a term present
                buffer.skip_space()
                if not grammar.Term.present(buffer):
                    errors.add(domain, 'expected a term', buffer.position)
                    return None
                
                production = grammar.Production.parse(buffer, errors)
                if not production:
                    return None
                
                productions.append(production)
            
            if not productions:
                errors.add(domain, 
                        'expected one or more productions', 
                        buffer.position)
                return None
        
        return Rule(name, type, productions, start_position)

    def link_references(self, rules: dict, errors: ErrorBuffer) -> bool:
        ''' Links other rules to this rule's production references

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
        
        success = True
        for production in self.productions:
            success = success and production.link_references(rules, errors)
        return success