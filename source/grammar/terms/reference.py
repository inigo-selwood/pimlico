from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer
from utilities import in_range


class Reference:

    def __init__(self, value: str, position: Position):
        self.value = value
        self.position = position
        self.type = 'reference'

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

        value = ''
        while True:
            if buffer.finished():
                break
                
            character = buffer.read()
            if (character != '_'
                    and not in_range(character, 'a', 'z') 
                    and not in_range(character, 'A', 'Z') 
                    and not in_range(character, '0', '9')):
                break

            value += character
            buffer.increment()
        
        assert value
        return Reference(value, start_position)