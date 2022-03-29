from grammar import Term


class Choice(Term):

    def __init__(self, values, position):
        self.values = values
        self.position = position

    @staticmethod
    def parse(buffer, errors):
        position = buffer.position

        values = []
        while True:

            term = Term.parse(buffer, errors, False)
            if not term:
                return None
            values.append(term)

            buffer.skip_space()
            if buffer.finished() or not buffer.read('|'):
                break

            buffer.skip_space()
            error_encountered = True
            if buffer.finished():
                errors.add('choice.parse',
                        'unexpected end-of-file in choice',
                        buffer.position)
            elif buffer.match('\n'):
                errors.add('choice.parse',
                        'unexpeted end-of-line in choice',
                        buffer.position)
            elif buffer.match(')'):
                errors.add('choice.parse',
                        'unexpected \')\' in choice',
                        buffer.position)
            else:
                error_encountered = False

            if error_encountered:
                return None

        assert values

        if len(values) == 1:
            return values[0]

        return Choice(values, position)
