from copy import copy

from grammar import Term
from grammar.terms import Choice


class Sequence(Term):

    def __init__(self, values, position):
        self.values = values
        self.position = position
        self.type = 'sequence'

    @staticmethod
    def parse(buffer, errors):
        position = copy(buffer.position)

        values = []
        while True:

            term = Choice.parse(buffer, errors)
            if not term:
                return None
            values.append(term)

            buffer.skip_space()
            if (buffer.finished()
                    or buffer.match('{')
                    or buffer.match('`')
                    or buffer.match('\n')
                    or buffer.match(')')):
                break

        if len(values) == 1:
            return values[0]

        return Sequence(values, position)
