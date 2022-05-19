from copy import copy

from hashlib import sha256

from utilities import escape_code, in_range, requires_escaping
from text import Position, ParseBuffer, ErrorBuffer, parse_escape_code
from grammar import Term


class Set(Term):

    domain = 'grammar.terms.set'

    def __init__(self, values: str, position: Position):
        self.values = values
        self.position = position
        self.type = 'set'

        context = sha256()
        ordered_characters = ''.join(sorted(values))
        context.update(ordered_characters.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        result = ''
        for character in self.values:
            if character == '`':
                result += '\\`'
            elif requires_escaping(character):
                result += escape_code(character)
            else:
                result += character
        
        if self.binding:
            return f'{self.binding}: `{result}`'
        else:
            return f'`{result}`'

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        r''' Parses a set

        Where a set has the format: `\`abcd\``

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a set term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        set: Set
            the parsed term
        '''

        start_position = copy(buffer.position)
        assert buffer.match('`', True)

        domain = f'{Set.domain}:parse'

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

            elif buffer.match('\\`', True):
                value += '`'

            elif buffer.match('\\'):
                code_position = copy(buffer.position)
                code = parse_escape_code(buffer)
                if not code:
                    errors.add(domain, 
                            'invalid escape code', 
                            code_position, 
                            buffer)
                    return None
                value += code
            
            elif buffer.match('`', True):
                break
            elif buffer.read() in value:
                errors.add(domain, 
                        'duplicate character', 
                        buffer.position, 
                        buffer)
                buffer.increment()
                valid = False

            else:
                value += buffer.read(True)

        if not valid:
            return None
        elif not value:
            errors.add(domain, 'empty', start_position, buffer)
            return None
        elif len(value) == 1:
            errors.add(domain, 'redundant (constant)', start_position, buffer)
            return None
        elif (len(value) > 2
                and (ord(value[-1]) - ord(value[0]) == len(value) - 1)):
            errors.add(domain, 'redundant (range)', start_position, buffer)
            return None

        return Set(value, start_position)
