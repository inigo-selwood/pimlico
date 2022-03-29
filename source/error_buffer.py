
class ErrorBuffer:

    def __init__(self):
        self.instances = []

    def add(self, section, message, position):
        self.instances.append((section, message, position))

    def serialize(self, buffer):
        for instance in self.instances:

            section, message, position = instance
            column = position.column
            line = position.line

            excerpt = buffer.line_text(line)

            pointer = ' ' * (column - buffer.line_indentation(line)) + '^'

            print(
                f'[{column}:{line}] ({section}) {message}\n'
                f'    {excerpt}\n'
                f'    {pointer}\n'
            )
