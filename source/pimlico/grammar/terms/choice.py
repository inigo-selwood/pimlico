from __future__ import annotations

from perivale import Buffer, ParseError

from pimlico.grammar import Term


class Choice(Term):

    def __init__(self, terms: list):
        self.terms = terms
    
    def __str__(self) -> str:
        return self.serialize()
    
    def serialize(self) -> str:
        terms = [term.serialize() for term in self.terms]
        return " | ".join(terms)

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Choice | None:
        
        terms = []
        while True:

            # Check there is, in fact, a term present
            if (buffer.finished() 
                    or buffer.match("\n") 
                    or not Term.present(buffer)):
                
                if not terms:
                    raise Exception("expected a term")
                else:
                    error = ParseError()
                    error.add_excerpt("expected a term",
                            buffer.excerpt())
                    errors.append(error)
                    return None
        
            # Parse the term
            term = Term.parse(buffer, errors)
            if term is None:
                return None
            terms.append(term)
            
            # Look for a vertical pipe
            buffer.skip_space()
            if not buffer.match("|", consume=True):
                break
        
            buffer.skip_space()

        # If it's a "choice" of one term, return that instead
        if len(terms) == 1:
            return terms[0]
        
        return Choice(terms)