from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer, parse_identifier
from utilities import in_range


class Reference:

    def __init__(self, value: str, position: Position):
        self.value = value
        self.position = position
        self.type = 'reference'
        self.bounds = (1, 1)

        context = sha256()
        context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()

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
