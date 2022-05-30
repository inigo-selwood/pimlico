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
        super(Reference, self).__init__()
        
        self.name = name
        self.term = None
        self.position = position
        self.type = 'reference'

        context = sha256()
        context.update(name.encode('utf-8'))
        self.hash = context.hexdigest()

    def __str__(self):
        return Term.decorate(self, self.name)
    
    @staticmethod
    def parse(buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Reference:

        position = copy(buffer.position)
        value = helpers.parse_identifier(buffer)
        return Reference(value, position)
    
    @Term.greedy_parser
    def match(self, buffer: text.Buffer) -> tuple:
        return self.rule.match(buffer)
    
    def link_rules(self, 
            rules: dict, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog,
            parent: grammar.Rule) -> bool:
        
        if self.name not in rules:
            errors.add(__name__, 
                    'undefined reference', 
                    buffer.excerpt(self.position))
            return False
        
        rule = rules[self.name]
        rule.used_by.append(parent)
        parent.uses.append(rule)
        self.rule = rule

        return True