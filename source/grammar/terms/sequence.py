from copy import copy

from hashlib import sha256

from utilities import in_range
from text import Position, ParseBuffer, ErrorBuffer

from grammar.terms import Choice


class Sequence:

    domain = 'grammar.terms.sequence'

    def __init__(self, values: list, position: Position):
        self.binding = ''
        self.values = values
        self.position = position
        self.type = 'sequence'
        self.bounds = (1, 1)

        context = sha256()
        value_hashes = [value.hash for value in values]
        for hash in value_hashes:
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer, root: bool = False):
        ''' Parses a sequence

        Where a sequence has the format: `term0 term1 ... termN`

        Sequences can also be enclosed in parentheses.

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a sequence term
        errors: ErrorBuffer
            buffer for reporting errors
        root: bool
            whether this sequence is the top-level of an expression

        Returns
        -------
        sequence: Sequence
            the parsed term
        '''

        character = buffer.read()
        assert (character == '\''
                or character == '['
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')
                or in_range(character, '0', '9')
                or character == '('
                or character == '`')

        domain = f'{Sequence.domain}:parse'
        start_position = copy(buffer.position)

        enclosed = False
        if not root and buffer.match('(', True):
            enclosed = True

            buffer.skip_space()
            if buffer.match(')', True):
                errors.add(domain, 'empty sequence', start_position)
                return None

        values = []
        valid = True
        while True:

            term = Choice.parse(buffer, errors)

            if not term:
                return None
            elif values and term.hash == values[-1].hash:
                errors.add(domain, 'redundant (instance hint)', term.position)
                valid = False
            else:
                values.append(term)

            buffer.skip_space()
            if (buffer.finished()
                    or buffer.match('\n')
                    or buffer.match('{')
                    or buffer.match(')')):
                break

        if not valid:
            return None
        elif enclosed and not buffer.match(')', True):
            errors.add(domain, 'expected \')\'', buffer.position)
            return None
        elif len(values) == 1:
            return values[0]

        return Sequence(values, start_position)
