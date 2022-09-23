from __future__ import annotations

from perivale import Buffer, Set, ParseException

import grammar
from grammar import Expression


class Rule:

    def __init__(self, name: str, type_hint: str, productions: list):
        self.name = name
        self.type_hint = type_hint
        self.productions = productions
        self.position = None

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Rule:
        position = buffer.copy_position()
        
        name = ""
        try:
            name = buffer.parse_set(Set.SNAKE, consume=True)
        except ParseException:
            raise Exception("expected a rule")
        
        type_hint = ""
        buffer.skip_space()
        if buffer.match("->", consume=True):

            buffer.skip_space()
            try:
                type_hint = buffer.parse_set(Set.SNAKE + ".", consume=True)
            except ParseException:
                message = "expected a type hint"
                errors.append(buffer.error(message))
                return None

        productions = []
        buffer.skip_space(include_newlines=True)
        while True:

            if not buffer.match(":=", consume=True):
                message = "expected a rule assignment (':=')"
                errors.append(buffer.error(message))
                return None
            elif not buffer.line_indentation() == 4:
                errors.append(buffer.error("rule definition under-indented"))
                return None
            
            buffer.skip_space()
            if not grammar.Term.present(buffer):
                errors.append(buffer.error("expected production terms"))
                return None
            
            sequence = grammar.terms.Sequence.parse(buffer, errors)
            if sequence is None:
                return None
            
            expression = None
            buffer.skip_space()
            if buffer.match("{"):
                expression = Expression.parse(buffer, errors)
                if expression is None:
                    return None
            productions.append((sequence, expression))
                
            buffer.skip_space(include_newlines=True)
            if not buffer.match(":="):
                break
        
        result = Rule(name, type_hint, productions)
        result.position = position
        return result

    def bind_references(self, 
            buffer: Buffer, 
            rules: dict, 
            errors: list) -> bool:
        
        success = True
        for production in self.productions:
            sequence, _ = production
            success = success and sequence.bind_references(buffer, rules, errors)
        return success