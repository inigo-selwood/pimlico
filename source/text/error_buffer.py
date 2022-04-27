from copy import copy

from .parse_buffer import Position, ParseBuffer


class ErrorBuffer:

    def __init__(self):
        self.instances = []

    def add(self, section: str, message: str, position: Position):
        ''' Adds an error message to the buffer

        Arguments
        ---------
        section: str
            the name of the section from which the error originated
        message: str
            some extra information about the error
        position: Position
            the point in the buffer where the error occured
        '''
        
        self.instances.append((section, message, copy(position)))

    def serialize(self, buffer: ParseBuffer) -> str:
        ''' Serializes all errors in the buffer

        Formats errors like so:

        ```
        [line:column] (section) message
            text
            ^
        ```

        Arguments
        ---------
        buffer: ParseBuffer
            used to get the text of the relevant line(s)

        Returns
        -------
        text: str
            the serialized error messages
        '''

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