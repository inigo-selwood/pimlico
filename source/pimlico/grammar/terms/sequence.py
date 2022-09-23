from __future__ import annotations

from perivale import Buffer

from grammar.terms import Choice
from grammar import Term


class Sequence(Term):

    def __init__(self, terms: list):
        super().__init__()
        self.terms = terms

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Sequence:
        position = buffer.copy_position()

        enclosed = buffer.match("(", consume=True)

        terms = []
        while True:
            
            buffer.skip_space()
            if (buffer.match(")") 
                    or buffer.match("{")
                    or buffer.finished() 
                    or buffer.match("\n")):
                break

            term = Choice.parse(buffer, errors)
            if term is None:
                return None
            terms.append(term)

        if enclosed and not buffer.match(")", consume=True):
            errors.append(buffer.error("expected ')' to match this", position))
            return None

        if enclosed and not terms:
            errors.append(buffer.error("empty sequence", position))
            return None

        if len(terms) == 1:
            return terms[0]

        result = Sequence(terms)
        return result
    
    def bind_references(self, 
            buffer: Buffer, 
            rules: dict, 
            errors: list) -> bool:
        
        success = True
        for term in self.terms:
            success = success and term.bind_references(buffer, rules, errors)
        return success