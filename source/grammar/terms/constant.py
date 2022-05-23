from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
import text
from text import helpers
import tools

from ..term import Term


class Constant(Term):

    def __init__(self, value: str, position: text.Position):
        self.value = value
        self.position = position
        self.type = 'constant'

        context = sha256()
        context.update(value.encode('utf-8'))
        self.hash = context.hexdigest()
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Constant:

        # Check for opening quote
        position = copy(buffer.position)
        if not buffer.match('\''):
            raise ValueError('expected \'\\\'\'')

        # Parse value
        value = helpers.parse_expression(buffer, 
                ('\'', '\''), 
                errors, 
                escape_codes=True)
        if value is None:
            return None

        # Extract text, discarding quotes -- check if empty
        value = value[1:-1]
        if not value:
            errors.add(__name__, 'empty', buffer.excerpt())
            return None

        return Constant(value, position)