from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
from grammar import terms
import text
import tools

from ..term import Term


class Intrinsic(Term):

    def __init__(self, variant: str, position: text.Position):
        super(Intrinsic, self).__init__()

        self.variant = variant
        self.position = position
        self.type = 'intrinsic'

        context = sha256()
        context.update(variant.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        instances = super(Intrinsic, self).__str__()
        return f'{self.variant}{instances}'
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Choice:
        
        if not buffer.match('__'):
            raise ValueError('expected a double-underscore')
        
        position = copy(buffer.position)

        variants = [
            '__character__',
            '__identifier__',
            '__indent__',
            '__integer__',
            '__newline__',
            '__number__',
        ]

        for variant in variants:
            if buffer.match(variant, consume=True):
                return Intrinsic(variant, position)
        
        # If we can't match it, try to parse it as a reference
        return terms.Reference.parse(buffer, errors)