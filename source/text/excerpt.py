from __future__ import annotations

from copy import copy

import text


class Excerpt:

    def __init__(self, 
            buffer: text.ParseBuffer, 
            position: text.Position = None):
        
        self.position = copy(position) if position else buffer.position
        self.text = buffer.line_text(self.position.line)
    
    def __str__(self):
        stripped_text = self.text.lstrip()
        indentation = len(self.text) - len(stripped_text)
        offset = ' ' * (self.position.column - indentation - 1)
        
        return (f'{stripped_text}'
                f'\n{offset}^')
    

