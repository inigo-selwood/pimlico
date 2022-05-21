from __future__ import annotations

import text
from text import helpers


class ParseBuffer:

    def __init__(self, buffer_text: str):
        if not buffer_text:
            raise ValueError('cannot create buffer from empty text')

        self.text = buffer_text
        self.length = len(buffer_text)
        self.position = text.Position()
        
        index = 0
        self.line_indices = []
        while index < self.length:
            self.line_indices.append(index)
            while index < self.length and buffer_text[index - 1] != '\n':
                index += 1
            index += 1
    
    def finished(self) -> bool:
        return self.position.index == self.length

    def increment(self, steps: int = 1):

        for _ in range(steps):

            # Stop when the end of the buffer has been reached
            if self.position.index >= self.length:
                return

            # If the current character is a newline, increment the line number
            # and reset the column
            character = self.text[self.position.index]
            if character == '\n':
                self.position.line += 1
                self.position.column = 1

            # Handle tabs by rounding up to the next multiple of 4 (+1)
            elif character == '\t':
                self.position.column = ((self.position.column + 3) / 4) * 4 + 1

            # Failing the above, and if the character wasn't format-related,
            # increment the column
            else:
                if helpers.in_range(self.text[self.position.index], ' ', '~'):
                    self.position.column += 1

            self.position.index += 1

            if self.position.index == self.length:
                self.position.column = -1
            elif self.text[self.position.index] == '\n':
                self.position.column = -1

    def line_text(self, line_number: int = 0) -> str:

        # Set line number to current if zero or less
        if line_number <= 0:
            line_number = self.position.line

        # Assert line number in range
        line_count = len(self.line_indices)
        if line_number < 1 or line_number > line_count:
            return ''

        # Evaluate start index
        start_index = 0
        if line_number > 1:
            start_index = self.line_indices[line_number - 1]

        # Evaluate end index
        end_index = self.length
        if line_number < line_count:
            end_index = self.line_indices[line_number] - 1
        elif self.length and self.text[self.length - 1] == '\n':
            end_index = self.length - 1

        return self.text[start_index:end_index]

    def read(self, consume: bool = False) -> str:

        # Check the buffer isn't finished
        if self.position.index == self.length:
            return ''

        # Get next character
        result = self.text[self.position.index]
        if consume:
            self.increment()
        
        return result

    def match(self, text: str, consume: bool = False) -> bool:

        # Check match wouldn't exceed buffer size
        length = len(text)
        if self.position.index + length > self.length:
            return False

        result = False
        if length == 1:
            result = self.text[self.position.index] == text
        else:
            end = self.position.index + length
            result = self.text[self.position.index:end] == text

        if result and consume:
            self.increment(length)

        return result

    def skip_space(self, include_newlines: bool = False):

        while True:

            if self.position.index == self.length:
                return

            character = self.text[self.position.index]

            if (character in ' \t\v\r'
                    or (include_newlines and character == '\n')):
                self.increment()

            else:
                return
    
    def excerpt(self, position: text.Position = None) -> text.Excerpt:
        if not position:
            position = self.position
        return text.Excerpt(self, position)