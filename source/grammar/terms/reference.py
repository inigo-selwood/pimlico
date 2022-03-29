from grammar import Term


class Reference(Term):

    def __init__(self, value, position):
        self.value = value
        self.position = position

    @staticmethod
    def parse(buffer, errors):

        def is_identifier_character(letter):
            index = ord(letter)
            return ((index >= ord('a') and index <= ord('z'))
                    or (index >= ord('A') and index <= ord('Z'))
                    or letter == '_')

        assert is_identifier_character(buffer.peek())

        position = buffer.position

        value = ''
        invalid_characters = []
        while True:
            if buffer.finished() or buffer.match(' '):
                break

            character = buffer.read()
            if is_identifier_character(character):
                value = value + character
            else:
                invalid_characters.append(character)

        if invalid_characters:
            errors.add('reference.parse',
                    f'invalid characters: {invalid_characters}',
                    position)
            return None

        return Reference(value, position)
