from __future__ import annotations

from copy import copy

import text


class Excerpt:

    def __init__(self, 
            buffer: text.Buffer, 
            position: text.Position = None):
        
        self.position = copy(position) if position else buffer.position
        self.text = buffer.line_text(self.position.line).strip()

        self.pointer_index = 0
        if position.column > 0:
            indentation = buffer.line_indentation(position.line)
            self.pointer_index = position.column - indentation - 1
        if position.column < 0:
            self.pointer_index = len(self.text.rstrip())
    
    def __str__(self):
        stripped_text = self.text.lstrip()
        offset = ' ' * self.pointer_index
        
        return (f'{stripped_text}'
                f'\n{offset}^')
    

