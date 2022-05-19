from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer, parse_identifier
from utilities import in_range
from grammar import Term


class Reference(Term):

    domain = 'grammar.reference'

    def __init__(self, value: str, position: Position):
        self.value = value
        self.rule = None
        self.position = position
        self.type = 'reference'

        context = sha256()
        context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        if self.binding:
            return f'{self.binding}: {self.value}'
        else:
            return self.value

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a reference

        Where a reference has the format: `nameOfSomeRule`

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a reference term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        reference: Reference
            the parsed term
        '''

        start_position = copy(buffer.position)

        value = parse_identifier(buffer)
        assert value
        
        return Reference(value, start_position)

    def link_references(self, 
            rules: dict, 
            parent_rule,
            buffer: ParseBuffer, 
            errors: ErrorBuffer) -> bool:
        
        ''' Links this reference's value to a rule

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
        parent_rule: Rule,
            the rule in which this reference is defined
        buffer: ParseBuffer
            the buffer used for parsing
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        success: bool
            whether or not the linking succeeded
        '''

        domain = f'{Reference.domain}:link'

        if self.value not in rules:
            errors.add(domain, 
                    f'undefined reference to rule \'{self.value}\'', 
                    self.position,
                    buffer)
            return False
        
        rule = rules[self.value]

        if parent_rule.name not in rule.exports:
            rule.exports[parent_rule.name] = parent_rule
        if rule.name not in parent_rule.imports:
            parent_rule.imports[rule.name] = rule
        self.rule = rule

        return True
