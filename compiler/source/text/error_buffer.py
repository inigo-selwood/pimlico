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
            indentation = buffer.line_indentation(position.line)
            excerpt = (text, copy(position), indentation)

            self.instances.append((section, message, excerpt))

        else:
            self.instances.append((section, message, None))

    def __str__(self) -> str:
        ''' Serializes all errors in the buffer

        Formats errors like so:

        ```
        [line:column] (section) message
        ```

        Or, if an excerpt was given:

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

            # If there's no excerpt object, we don't need to print any text or 
            # caret
            if not excerpt:
                return f'({section}) {message}'

            else:
                
                text, position,indentation = excerpt

                # Evaluate how much padding the caret needs on its left-hand 
                # side, to line up with the proper column in the text excerpt
                pointer_offset = ''
                if position.column == -1:
                    pointer_offset = ' ' * len(excerpt)
                else:
                    pointer_offset = ' ' * (position.column - indentation - 1)

                return (f'{position.__str__()} ({section}) {message}'
                        f'\n    {text}'
                        f'\n    {pointer_offset}^')