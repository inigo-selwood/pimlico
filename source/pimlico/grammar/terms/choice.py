from __future__ import annotations

from perivale import Buffer

from grammar import Term


class Choice(Term):

    def __init__(self, terms: list):
        super().__init__()
        self.terms = terms
    
    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Choice:
        if buffer.finished() or buffer.match("\n"):
            raise Exception("expected a choice")
        
        terms = []
        last_pipe_position = None
        while True:

            term = Term.parse(buffer, errors)
            if term is None:
                return None
            terms.append(term)

            buffer.skip_space()
            last_pipe_position = buffer.copy_position()
            if not buffer.match("|", consume=True):
                break

            buffer.skip_space()
            if buffer.finished():
                message = "unexpected end-of-file following choice operator"
                errors.append(buffer.error(message, last_pipe_position))
                return None
            elif buffer.match("\n"):
                message = "unexpected newline following choice operator"
                errors.append(buffer.error(message, last_pipe_position))
                return None
        
        if len(terms) == 1:
            return terms[0]
        
        result = Choice(terms)
        return result
    
    def bind_references(self, 
            buffer: Buffer, 
            rules: dict, 
            errors: list) -> bool:
        
        success = True
        for term in self.terms:
            success = success and term.bind_references(buffer, rules, errors)
        return success