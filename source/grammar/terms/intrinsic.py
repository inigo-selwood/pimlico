from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
from grammar import terms
import text
import tools

from ..term import Term


class Intrinsic(Term):

    def __init__(self, type: str, position: text.Position):
        self.type = type
        self.position = position
        self.type = 'intrinsic'

        context = sha256()
        context.update(type.encode('utf-8'))
        self.hash = context.hexdigest()
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Choice:
        
        if not buffer.match('__'):
            raise ValueError('expected a double-underscore')
        
        position = copy(buffer.position)

        types = [
            '__character__',
            '__identifier__',
            '__indent__',
            '__integer__',
            '__newline__',
            '__number__',
        ]

        for type in types:
            if buffer.match(type, consume=True):
                return Intrinsic(type, position)
        
        # If we can't match it, try to parse it as a reference
        return terms.Reference.parse(buffer, errors)