from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
from grammar import terms
import text
import tools

from ..term import Term


class Sequence(Term):

    def __init__(self, sequence_terms: list, position: text.Position):
        super(Sequence, self).__init__()

        self.terms = sequence_terms
        self.position = position
        self.type = 'sequence'

        context = sha256()
        value_hashes = [term.hash for term in sequence_terms]
        for hash in value_hashes:
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        result = ''
        
        count = len(self.terms)
        for index in range(count):

            term = self.terms[index]

            enclosed = False
            if term.type == 'sequence':
                result += '('
                enclosed = True

            result += term.__str__()

            if enclosed:
                result += ')'
                result += super(Sequence, term).__str__()

            if index + 1 < count:
                result += ' '
            index += 1
        
        return result

    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Sequence:
        
        position = copy(buffer.position)

        if not grammar.Term.present(buffer):
            raise ValueError('expected a term')

        # Check for parenthesis enclosure
        enclosed = False
        if buffer.match('(', True):
            enclosed = True

            buffer.skip_space()
            if buffer.match(')', True):
                errors.add(__name__, 
                        'empty sequence', 
                        buffer.excerpt(position))
                return None

        values = []
        valid = True
        while True:

            # Parse term
            term = terms.Choice.parse(buffer, errors)
            if not term:
                return None
            
            # Check value not duplicated
            elif values and term.hash == values[-1].hash:
                errors.add(__name__, 
                        'redundant (instance hint)', 
                        buffer.excerpt(term.position,))
                valid = False
            
            else:
                values.append(term)

            # Break when a newline, expression, or enclosure are reached
            buffer.skip_space()
            if (buffer.finished()
                    or buffer.match('\n')
                    or buffer.match('{')
                    or buffer.match(')')):
                break

        if not valid:
            return None
        
        # Make sure closing bracket present, if needed
        elif enclosed and not buffer.match(')', True):
            errors.add(__name__, 'expected \')\'', buffer.excerpt())
            return None
        
        # If it's a sequence of one term, return that term
        elif len(values) == 1:
            return values[0]

        return Sequence(values, position)