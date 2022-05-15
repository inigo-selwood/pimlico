from copy import copy

from hashlib import sha256

from utilities import in_range
from text import Position, ParseBuffer, ErrorBuffer
from grammar import Term


class Constant(Term):

    domain = 'grammar.terms.constant'

    def __init__(self, value: str, position: Position):
        self.value = value
        self.position = position
        self.type = 'constant'

        context = sha256()
        context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        result = ''

        value = ''
        for letter in self.value:
            if letter == '\'':
                value += '\\\''
            else:
                value += letter
        
        if self.binding:
            return f'{self.binding}: \'{value}\''
        else:
            return f'\'{value}\''

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a constant

        Where a constant has the format: `'some text'`

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a constant term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        constant: Constant
            the parsed term
        '''

        start_position = copy(buffer.position)
        assert buffer.match('\'', True)

        domain = f'{Constant.domain}:parse'

        value = ''
        valid = True
        while True:

            if buffer.finished():
                errors.add(domain, 
                        'unexpected end-of-file', 
                        buffer.position, 
                        buffer)
                return None
            elif buffer.match('\n'):
                errors.add(domain, 
                        'unexpected newline', 
                        buffer.position, 
                        buffer)
                return None
            elif not in_range(buffer.read(), ' ', '~'):
                errors.add(domain, 
                        'invalid character', 
                        buffer.position, 
                        buffer)
                buffer.increment()
                valid = False

            elif buffer.match('\\\'', True):
                value += '\''
            elif buffer.match('\'', True):
                break

            else:
                value += buffer.read(True)

        if not valid:
            return None
        elif not value:
            errors.add(domain, 'empty', start_position, buffer)
            return None

        return Constant(value, start_position)
