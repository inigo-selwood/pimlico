from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
import text
from text import helpers
import tools

from ..term import Term


class Reference(Term):

    def __init__(self, name: str, position: text.Position):
        self.name = name
        self.position = position
        self.type = 'reference'

        context = sha256()
        context.update(name.encode('utf-8'))
        self.hash = context.hexdigest()
    
    @staticmethod
    def parse(buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Reference:

        position = copy(buffer.position)
        value = helpers.parse_identifier(buffer)
        return Reference(value, position)