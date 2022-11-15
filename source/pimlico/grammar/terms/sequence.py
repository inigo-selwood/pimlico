from __future__ import annotations

from perivale import Buffer, ParseError

from pimlico.grammar import Term

from .choice import Choice


class Sequence(Term):

    def __init__(self, terms: list):
        self.terms = terms
    
    def __str__(self) -> str:
        return self.serialize()
    
    def serialize(self) -> str:
        terms = [term.serialize() for term in self.terms]
        return " ".join(terms)

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Sequence | None:
        
        term_position = buffer.copy_position()
        
        enclosed = False
        if buffer.match("(", consume=True):
            enclosed = True
            buffer.skip_space()

        terms = []
        while True:
        
            if not Term.present(buffer):
                raise Exception("expected a term")
            
            term = Choice.parse(buffer, errors)
            if term is None:
                return None
            terms.append(term)

            buffer.skip_space()
            if buffer.finished() or buffer.read() in "\n{)":
                break
        
        if enclosed and not buffer.match(")", consume=True):
            error = ParseError()
            error.add_excerpt("expected ')'",
                    buffer.excerpt())
            error.add_excerpt("to match this",
                    buffer.excerpt(term_position))
            errors.append(error)
            return None
        
        if len(terms) == 1:
            return terms[0]

        return Sequence(terms)
            

