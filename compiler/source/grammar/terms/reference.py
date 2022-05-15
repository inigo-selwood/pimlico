from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer, parse_identifier
from utilities import in_range
from grammar import Term


class Reference(Term):

    domain = 'grammar.reference'

    def __init__(self, value: str, position: Position):
        self.binding = ''
        self.value = value
        self.term = None
        self.position = position
        self.type = 'reference'
        self.bounds = (1, 1)

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
            buffer: ParseBuffer, 
            errors: ErrorBuffer) -> bool:
        
        ''' Links this reference's value to a rule

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
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
        
        self.term = rules[self.value]
        return True
