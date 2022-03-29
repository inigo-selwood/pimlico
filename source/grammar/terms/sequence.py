from grammar import Term
from grammar.terms import Choice


class Sequence(Term):

    def __init__(self):
        self.terms = []

    @staticmethod
    def parse(buffer, errors):
        position = buffer.position

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
                    or buffer.peek('\n')
                    or buffer.peek(')')):
                break

        if len(values) == 1:
            return values[0]

        return Sequence(values, position)
