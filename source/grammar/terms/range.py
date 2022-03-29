from copy import copy 

from grammar import Term


class Range(Term):

    def __init__(self, values, position):
        self.values = values
        self.position = position
        self.type = 'range'

    @staticmethod
    def parse(buffer, errors):

        def search(character, limit=5):

            steps = 0
            while True:
                if buffer.finished():
                    return -1
                elif buffer.read() == character:
                    return steps

                steps += 1
                if steps > limit:
                    return -1

        def fail():
            delta = search(']', 8)
            if delta > 0:
                buffer.increment(delta)
            return None

        assert buffer.match('[', True)

        position = copy(buffer.position)

        # Parse lower bound
        buffer.skip_space()
        lower = ''
        if buffer.match('-'):
            errors.add('range.parse', 'missing lower bound', buffer.position)
            return fail()
        elif buffer.match(r'\-', True):
            lower = '-'
        else:
            lower = buffer.read()

        if not lower:
            errors.add('range.parse',
                    'unexpected end-of-file',
                    buffer.position)
            return None

        # Check for a cheeky seperator
        buffer.skip_space()
        if not buffer.match('-', True):
            errors.add('range.parse', 'expected \'-\'', buffer.position)
            return fail()

        # Parse upper bound
        buffer.skip_space()
        upper = ''
        if buffer.match(']'):
            errors.add('range.parse', 'missing upper bound', buffer.position)
        elif buffer.match(r'\]', True):
            upper = ']'
        else:
            upper = buffer.read()

        if not upper:
            errors.add('range.parse',
                    'unexpected end-of-file',
                    buffer.position)
            return None

        # Check for closing bracket
        buffer.skip_space()
        if not buffer.match(']', True):
            errors.add('range.parse', 'expected \']\'', buffer.position)
            return fail()

        # Check the range of characters actually makes sense
        lowerIndex = ord(lower)
        upperIndex = ord(upper)
        if lowerIndex >= upperIndex:
            errors.add('range.parse',
                    'illogical character range',
                    buffer.position)
            return None

        # Check the range values given are strictly vanilla
        for index in [lowerIndex, upperIndex]:
            if index < ord(' ') or index > ord('~'):
                errors.add('range.parse',
                        'invalid character in range',
                        position)

        return Range((lower, upper), position)
