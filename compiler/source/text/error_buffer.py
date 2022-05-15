from copy import copy

from .parse_buffer import Position, ParseBuffer


class ErrorBuffer:

    def __init__(self):
        self.instances = []

    def add(self, section: str, message: str, position: Position = None):
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
        
        new_position = copy(position) if position else None
        self.instances.append((section, message, new_position))

    def serialize(self, buffer: ParseBuffer = None) -> str:
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

            if not position:
                print(f'({section}) {message}')

            else:
                assert self.buffer

                excerpt = buffer.line_text(line)

                pointer = ''
                if column == -1:
                    pointer = ' ' * len(excerpt)
                else:
                    offset = column - buffer.line_indentation(line) - 1
                    pointer = ' ' * offset

                print(f'{position.serialize()} ({section}) {message}\n'
                        f'    {excerpt}\n'
                        f'    {pointer}^')