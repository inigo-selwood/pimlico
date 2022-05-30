from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
import text
from text import helpers
import tools

from ..term import Term


class Range(Term):

    def __init__(self, values: tuple, position: text.Position):
        super(Range, self).__init__()

        self.values = values
        self.position = position
        self.type = 'range'
        
        context = sha256()
        for value in values:
            context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()

    def __str__(self):
        escape_codes = {
            ' ': '\' \'',
            ']': '\\]',
        }

        lower, upper = self.values
        lower = helpers.escape(lower, custom_codes=escape_codes)
        upper = helpers.escape(upper, custom_codes=escape_codes)
        
        return Term.decorate(self, f'[{lower}{upper}]')
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Sequence:

        position = copy(buffer.position)

        def parse_range_value() -> str:

            # Check for EOF
            if buffer.finished():
                errors.add(__name__, 
                        'unexpected end-of-file', 
                        buffer.excerpt())
                return None
            
            # Don't allow newlines
            elif buffer.match('\n'):
                errors.add(__name__, 'unexpected newline', buffer.excerpt())
                return None
            
            # We can't have reached the closing bracket yet
            elif buffer.match(']', consume=True):
                errors.add(__name__, 
                        'too few characters', 
                        buffer.excerpt(position))
                return None

            # Handle escaped spaces
            elif buffer.match('\' \'', consume=True):
                return ' '
            
            # Escaped closing brackets too
            elif buffer.match('\\]', consume=True):
                return ']'
            
            # Check it's valid
            elif not helpers.in_range(buffer.read(), ' ', '~'):
                errors.add(__name__, 'invalid character', buffer.excerpt())
                buffer.increment()
                return None

            return buffer.read(consume=True)

        # Check for bracket
        if not buffer.match('[', consume=True):
            raise ValueError('expected \'[\'')

        # Read lower value
        buffer.skip_space()
        lower = parse_range_value()
        if not lower:
            return None

        # Read upper value
        buffer.skip_space()
        upper = parse_range_value()
        if not upper:
            return None

        # Make sure EOF not reached
        buffer.skip_space()
        if buffer.finished():
            errors.add(__name__, 
                    'unexpected end-of-file', 
                    buffer.excerpt())
            return None
        
        # Check for newline
        elif buffer.match('\n'):
            errors.add(__name__, 'unexpected newline', buffer.excerpt())
            return None
        
        # Find closing bracket
        elif not buffer.match(']', consume=True):
            errors.add(__name__, 'expected \']\'', buffer.excerpt())
            return None
        
        lower_index = ord(lower)
        upper_index = ord(upper)

        if lower_index > upper_index:
            errors.add(__name__, 
                    f'illogical (\'{lower}\' > \'{upper}\')',
                    buffer.excerpt(position))
            return None
        
        elif lower_index == upper_index:
            errors.add(__name__, 
                    f'redundant (constant)',
                    buffer.excerpt(position))
            return None

        return Range((lower, upper), position)
    
    @Term.greedy_parser
    def match(self, buffer: text.Buffer) -> tuple:
        character = buffer.read()
        lower, upper = self.values
        
        if helpers.in_range(character, lower, upper):
            buffer.increment()
            return (True, character)
        return (False, '')