from copy import copy

from utilities import in_range


class Position:

    def __init__(self):
        self.index = 0
        self.line = 1
        self.column = 1
    
    def serialize(self) -> str:
        ''' Formats the position
        Returns
        -------
        value: str
            the serialized position
        '''

        return f'[{self.line}:{self.column}]'


class ParseBuffer:

    def __init__(self, text: str):
        ''' Constructs the buffer object

        Arguments
        ---------
        text: str
            text to fill the buffer with
        '''

        self.text = text
        self.length = len(text)

        self.index_indentations = []

        self.position = Position()
        if self.length == 0:
            self.position.column = -1
            self.position.line = -1
            return
        elif self.text[0] == '\n':
            self.position.column = -1

        # Record the index at which each line starts, and the indentation level
        # of those lines
        index = 0
        line_start_index = 0
        while index < self.length:
            line_start_index = index

            indentation = 0
            while True:
                if index == self.length:
                    break

                if text[index] == '\t':
                    indentation = ((indentation + 4) & ~0x03)
                elif text[index] == ' ':
                    indentation += 1
                else:
                    break

                index += 1

            self.index_indentations.append((line_start_index, indentation))

            # Increment to the next newline
            while index < self.length and text[index] != '\n':
                index += 1

            index += 1

    def _get_character(self) -> str:
        ''' Gets a single character from the buffer
        Returns
        -------
        character: str
            the character found, or None if the end of the buffer has already
            been reached
        '''

        if self.position.index == self.length:
            return None
        return self.text[self.position.index]

    def finished(self) -> bool:
        ''' Checks whether the buffer is finished
        Returns
        -------
        finished: bool
            True if the end of the buffer has been reached
        '''

        return self.position.index == self.length

    def increment(self, steps: int = 1):
        ''' Increments the position in the buffer, updating the line and column
        Arguments
        ---------
        steps: int 
            the number of steps to increment the position by
        '''

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
                if in_range(self.text[self.position.index], ' ', '~'):
                    self.position.column += 1

            self.position.index += 1

            if self.position.index == self.length:
                self.position.column = -1
            elif self.text[self.position.index] == '\n':
                self.position.column = -1

    def read(self, consume: bool = False) -> str:
        ''' Read the next character from the buffer, optionally incrementing
        Arguments
        ---------
        consume: bool
            if true, increments past the character just read
        Returns
        -------
        character: str
            the next character in the buffer
        '''

        if self.position.index == self.length:
            return None

        result = self.text[self.position.index]
        if consume:
            self.increment()
        
        return result

    def line_indentation(self, line_number: int = 0) -> int:
        ''' Gets the indentation of the current line (default), or a given one
        Arguments
        ---------
        line_number: int
            the index of the line to get the indentation of (optional)
        
        Returns
        -------
        indentation: int
            the indentation level in steps, where a tab is 4, and a space is 1
        '''

        if line_number <= 0:
            line_number = self.position.line

        line_count = len(self.index_indentations)
        if line_number < 1 or line_number > line_count:
            return None

        return self.index_indentations[line_number - 1][1]

    def line_text(self, line_number: int = 0) -> str:
        ''' Gets the text of the current line (default), or a given one
        Arguments
        ---------
        line_number: int
            the index of the line to get the text from (optional)
        
        Returns
        -------
        text: str
            the text of the current line, from newline to newline
        '''

        if line_number <= 0:
            line_number = self.position.line

        line_count = len(self.index_indentations)
        if line_number < 1 or line_number > line_count:
            return ''

        start_index = 0
        if line_number > 1:
            start_index = self.index_indentations[line_number - 1][0]

        end_index = self.length
        if line_number < line_count:
            end_index = self.index_indentations[line_number][0] - 1
        elif self.length and self.text[self.length - 1] == '\n':
            end_index = self.length - 1

        line = self.text[start_index:end_index]

        return line.strip()

    def match(self, text: str, consume: bool = False) -> bool:
        ''' Checks for a string match, optionally consuming it
        Arguments
        ---------
        text: str
            the text to try and match
        consume: bool
            whether to increment past the value (if matched), optional
        
        Returns
        -------
        match: bool
            whether the string matched
        '''

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
    
    def seek(self, text: str, consume: bool = False, limit: int = -1) -> bool:
        ''' Looks ahead a limited number of steps to try and match a string
        **Note:** Whitespace characters don't count towards the limit
        Arguments
        ---------
        text: str
            the text to try and match
        consume: bool
            if true, increments past any matched value
        limit: int
            the maximum number of characters to look ahead (not inclusive), 
            before abandoning the search
        '''

        start_position = copy(self.position)
        index = 0
        result = False
        while True:
            
            # Increment index (if applicable)
            if limit > 0:
                if index >= limit:
                    break
                index += 1

            # Try to match the text
            self.skip_space()
            if self.position.index == self.length:
               break
            elif self.match(text, consume):
                result = True
                break
            
            self.increment()
        
        # If the search failed, reset the position
        if not result or not consume:
            self.position = start_position
        
        return result

    def skip_line(self):
        ''' Skips to the next newline (or EOF)
        '''

        self.position.column = -1

        line_count = len(self.index_indentations)
        if self.position.line >= line_count:
            self.position.index = self.length

        else:
            index = self.index_indentations[self.position.line][0]
            self.position.index = index - 1

    def skip_space(self, include_newlines: bool = False):
        ''' Skips whitespace characters, optionally including newlines
        Arguments
        ---------
        include_newlines: bool
            if true, skips newline characters as well
        '''

        while True:

            if self.position.index == self.length:
                return

            character = self.text[self.position.index]
            if character == '#':
                self.skip_line()

            elif (character in [' ', '\t', '\r', '\v']
                    or (include_newlines and character == '\n')):
                self.increment()

            else:
                return