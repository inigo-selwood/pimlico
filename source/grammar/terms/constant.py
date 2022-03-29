from copy import copy

from grammar import Term


class Constant(Term):

    def __init__(self, value, position):
        self.value = value
        self.position = position
        self.type = 'constant'

    def parse(buffer, errors):
        position = copy(buffer.position)

        assert buffer.match('\'', True)

        value = ''
        while True:
            if buffer.finished() or buffer.match('\''):
                break

            character = buffer.peek()
            index = ord(character)
            if index < ord(' ') or index > ord('~'):
                errors.add('constant.parse',
                        'unexpected character in constant',
                        buffer.position)
                break

            value = value + buffer.read()

        if not buffer.match('\'', True):
            errors.add('constant.parse', 'expected \'\\\'\'', buffer.position)
            return None
        elif not value:
            errors.add('constant.parse', 'empty constant', position)
            return None

        return Constant(value, position)
