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
        self.value = values
        self.position = position
        self.type = 'set'

        context = sha256()
        ordered_characters = ''.join(sorted(values))
        context.update(ordered_characters.encode('utf-8'))
        self.hash = context.hexdigest()
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Constant:

        # Check for backtick
        position = copy(buffer.position)
        if not buffer.match('`'):
            raise ValueError('expected \'`\'')

        # Parse value
        value = helpers.parse_expression(buffer, 
                ('`', '`'), 
                errors, 
                escape_codes=True)
        if value is None:
            return None

        # Discard backticks, make sure there are enough values
        value = value[1:-1]
        if not value:
            errors.add(__name__, 'empty', buffer.excerpt(position))
            return None
        
        # Check the set couldn't have been a constant
        elif len(value) == 1:
            errors.add(__name__, 
                    'redundant (constant)', 
                    buffer.excerpt(position))
            return None
        
        # Check for duplicates
        for letter in value:
            if value.count(letter) > 1:
                errors.add(__name__, 
                        'contains duplicates', 
                        buffer.excerpt(position))
                return None

        return Set(value, position)