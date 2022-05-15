from copy import copy

from hashlib import sha256

from utilities import in_range
from text import Position, ParseBuffer, ErrorBuffer
from grammar import Term


class Range(Term):

    domain = 'grammar.terms.range'

    def __init__(self, values: tuple, position: Position):
        self.binding = ''
        self.values = values
        self.position = position
        self.type = 'range'
        self.hash = hash(values)
        self.bounds = (1, 1)

        context = sha256()
        for value in values:
            context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):

        lower, upper = self.values
        if lower == ']':
            lower = '\\]'
        elif lower == ' ':
            lower = '\' \''
        
        if upper == ']':
            upper = '\\]'
        elif upper == ' ':
            upper = '\' \''

        result = f'[{lower}{upper}]'

        if self.binding:
            return f'{self.binding}: {result}'
        else:
            return result

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a range

        Where a range has the format: `[az]`

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a range term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        range: Range
            the parsed term
        '''

        domain = f'{Range.domain}:parse'
        start_position = copy(buffer.position)

        def read_bound() -> str:
            ''' Extracts a single bound value

            Returns
            -------
            bound: str
                the parsed bound
            '''

            buffer.skip_space()
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
            elif buffer.match(']', True):
                errors.add(domain, 
                        'too few characters', 
                        start_position, 
                        buffer)
                return None
            elif not in_range(buffer.read(), ' ', '~'):
                errors.add(domain, 
                        'invalid character', 
                        buffer.position, 
                        buffer)
                buffer.increment()
                return None

            elif buffer.match('\' \'', True):
                return ' '
            elif buffer.match('\\]', True):
                return ']'

            return buffer.read(True)

        assert buffer.match('[', True)

        lower = read_bound()
        if not lower:
            return None

        upper = read_bound()
        if not upper:
            return None

        buffer.skip_space()
        if buffer.finished():
            errors.add(domain, 
                    'unexpected end-of-file', 
                    buffer.position, 
                    buffer)
            return None
        elif buffer.match('\n'):
            errors.add(domain, 'unexpected newline', buffer.position, buffer)
            return None
        elif not buffer.match(']', True):
            errors.add(domain, 'expected \']\'', buffer.position, buffer)
            return None

        return Range((lower, upper), start_position)