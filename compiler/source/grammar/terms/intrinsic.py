from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer, parse_identifier
from utilities import in_range
from grammar import Term


class Intrinsic(Term):

    domain = 'grammar.intrinsic'

    def __init__(self, value: str, position: Position):
        self.value = value
        self.term = None
        self.position = position
        self.type = 'intrinsic'

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
        ''' Parses a intrinsic

        Where a intrinsic has the format: `nameOfSomeRule`

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a intrinsic term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        intrinsic: Intrinsic
            the parsed term
        '''

        start_position = copy(buffer.position)

        assert buffer.match('__')

        intrinsics = [
            '__character__',
            '__identifier__',
            '__indent__',
            '__integer__',
            '__newline__',
            '__number__',
        ]

        section = f'{Intrinsic.domain}:parse'

        value = parse_identifier(buffer)
        if value not in intrinsics:
            errors.add(section, 'not recognized', start_position, buffer)
            return None
        
        return Intrinsic(value, start_position)
