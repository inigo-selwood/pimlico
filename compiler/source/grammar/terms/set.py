from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
import text
from text import helpers
import tools

from ..term import Term


class Set(Term):

    def __init__(self, values: str, position: text.Position):
        super(Set, self).__init__()

        self.values = values
        self.position = position
        self.type = 'set'

        context = sha256()
        ordered_characters = ''.join(sorted(values))
        context.update(ordered_characters.encode('utf-8'))
        self.hash = context.hexdigest()

    def __str__(self):
        values = helpers.escape(self.values, custom_codes={'`': '\\`'})
        instances = super(Set, self).__str__()
        return f'`{values}`{instances}'
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Constant:

        # Check for backtick
        position = copy(buffer.position)
        if not buffer.match('`'):
            raise ValueError('expected \'`\'')

        # Parse values
        values = helpers.parse_expression(buffer, 
                ('`', '`'), 
                errors, 
                escape_codes=True)
        if values is None:
            return None

        # Discard backticks, make sure there are enough values
        values = values[1:-1]
        if not values:
            errors.add(__name__, 'empty', buffer.excerpt(position))
            return None
        
        # Check the set couldn't have been a constant
        elif len(values) == 1:
            errors.add(__name__, 
                    'redundant (constant)', 
                    buffer.excerpt(position))
            return None
        
        # Check for duplicates
        for letter in values:
            if values.count(letter) > 1:
                errors.add(__name__, 
                        'contains duplicates', 
                        buffer.excerpt(position))
                return None

        return Set(values, position)
    
    @Term.greedy_parser
    def match(self, buffer: text.Buffer) -> tuple:
        character = buffer.read()
        
        if character in self.values:
            buffer.increment()
            return (True, character)
        return (False, '')