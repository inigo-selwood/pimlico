from copy import copy 

from grammar import Term


class Set(Term):

    def __init__(self, characters, position):
        self.characters = characters
        self.position = position

    @staticmethod
    def parse(buffer, errors):
        assert buffer.match('`', True)

        position = copy(buffer.position)

        characters = ''
        errors_found = False
        while True:

            if buffer.finished():
                errors.add('set.parse',
                        'unexpected end-of-file',
                        buffer.position)
                return None

            elif buffer.match(r'\`', True):
                characters = characters + '`'

            elif buffer.match('`'):
                break

            else:
                character = buffer.read()
                index = ord(character)

                if index < ord(' ') or index > ord('~'):

                    value = character.encode('unicode_escape')
                    errors.add('set.parse',
                            f'unexpected character {value}',
                            buffer.position)
                    errors_found = True

                else:
                    characters = characters + character

        if not buffer.match('`', True):
            errors.add('set.parse',
                    'expected \'`\'',
                    buffer.position)
            return None

        if not characters:
            errors.add('set.parse',
                    'empty set',
                    buffer.position)
            return None

        return Set(characters, position)
