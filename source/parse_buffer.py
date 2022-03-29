

class Position:

    def __init__(self):
        self.index = 0
        self.line = 1
        self.column = 1

class ParseBuffer:

    def __init__(self, text: str):
        self.text = text
        self.position = Position()
        self.length = len(text)

        self.index_indentations = []
        line_start_index = 0

        index = 0
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

            while index < self.length and text[index] != '\n':
                index += 1

            index += 1

    def _get_character(self):
        if self.position.index == self.length:
            return None
        return self.text[self.position.index]

    def finished(self):
        return self.position.index == self.length

    def increment(self, steps=1):

        for _ in range(steps):

            if self.position.index >= self.length:
                return

            character = self.text[self.position.index]
            if character == '\n':
                self.position.line += 1
                self.position.column = 1

            elif character == '\t':
                self.position.column = ((self.position.column + 3) / 4) * 4 + 1

            else:
                if (self.text[self.position.index] >= ' '
                        and self.text[self.position.index] <= '~'):
                    self.position.column += 1

            self.position.index += 1

    def read(self):
        if self.position.index == self.length:
            return None

        result = self.text[self.position.index]
        self.increment()
        return result

    def peek(self):
        if self.position.index == self.length:
            return None
        return self.text[self.position.index]

    def line_indentation(self, line_number=0):
        if line_number <= 0:
            line_number = self.position.line

        line_count = len(self.index_indentations)
        if line_number < 1 or line_number > line_count:
            return None

        return self.index_indentations[line_number - 1][1]

    def line_text(self, line_number=0):

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

    def match(self, text, consume=False):
        if self.position.index >= self.length:
            return False

        length = len(text)
        result = False
        if length == 1:
            result = self.text[self.position.index] == text
        else:
            end = self.position.index + length
            result = self.text[self.position.index:end] == text

        if result and consume:
            self.increment(length)

        return result

    def skip_line(self):

        line_count = len(self.index_indentations)
        if self.position.line >= line_count:
            column = self.length - self.index_indentations[-1][0]
            self.position.column = column
            self.position.index = self.length

        else:
            index = self.index_indentations[self.position.line][0]
            self.position.index = index

            self.position.line += 1
            self.position.column = 0

    def skip_space(self):

        while True:

            if self.position.index == self.length:
                return

            character = self.text[self.position.index]
            if character == '#':
                self.skip_line()

            elif character in [' ', '\t', '\r', '\v']:
                self.increment()

            else:
                return
