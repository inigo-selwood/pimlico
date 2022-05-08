from text import ParseBuffer, ErrorBuffer, parse_bounded_text

import grammar


class Production:
    
    domain = 'grammar.production'

    def __init__(self, term, expression: str):
        self.term = term
        self.expression = expression

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):

        domain = f'{Production.domain}:parse'

        if not grammar.Term.present(buffer):
            errors.add(domain, 'expected a term', buffer.position)
            return None
        
        term = grammar.terms.Sequence.parse(buffer, errors, True)
        if not term:
            return None
        
        expression = ''
        buffer.skip_space()
        if buffer.match('{'):
            expression = parse_bounded_text(buffer, errors, '{', '}')
            if not expression:
                return None
        
        return Production(term, expression)