from copy import copy

from .parse_buffer import Position, ParseBuffer


class ErrorBuffer:

    def __init__(self):
        self.instances = []

    def add(self, 
            section: str, 
            message: str, 
            position: Position = None, 
            buffer: ParseBuffer = None):
        
        ''' Adds an error message to the buffer

        Arguments
        ---------
        section: str
            the name of the section from which the error originated
        message: str
            some extra information about the error
        position: Position
            the point in the buffer where the error occured
        buffer: ParseBuffer
            the buffer to extract a quote from, if neccessary
        '''
        
        if position:
            assert buffer

            text = buffer.line_text(position.line)
            excerpt = (text, position)
            self.instances.append((section, message, excerpt))

        else:
            self.instances.append((section, message, None))

    def __str__(self) -> str:
        ''' Serializes all errors in the buffer

        Formats errors like so:

        ```
        [line:column] (section) message
            text
            ^
        ```

        Returns
        -------
        text: str
            the serialized error messages
        '''

        for instance in self.instances:

            section, message, excerpt = instance

            if not excerpt:
                return f'({section}) {message}'

            else:
                
                text, position = excerpt

                pointer = ''
                if position.column == -1:
                    pointer = ' ' * len(excerpt)
                else:
                    pointer = ' ' * position.column

                return (f'{position.__str__()} ({section}) {message}\n'
                        f'    {text}\n'
                        f'    {pointer}^')