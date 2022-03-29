
class ErrorBuffer:

    def __init__(self):
        self.instances = []

    def add(self, section, message, position):
        self.instances.append((section, message, position))

    def print(self, buffer):
        for instance in self.instances:

            section, message, position = instance
            column, line = position

            excerpt = buffer.line_text(line)

            pointer = ' ' * (column - buffer.line_indentation(line)) + '^'

            print(f'[{column}:{line}] ({section}) {message}')
            print(f'    {excerpt}')
            print(f'    {pointer}')
