from __future__ import annotations

import text


class ErrorLog:

    def __init__(self):
        self.values = []
    
    def __str__(self):
        result = ''

        count = len(self.values)
        index = 0

        for value in self.values:
            
            if len(value) == 2:
                module, message = value
                result += f'({module}) {message}'
            else:
                module, message, excerpt = value
                result += (f'({module}) {excerpt.position} {message}'
                        f'\n{excerpt}')

            if index + 1 < count:
                result += '\n'
            index += 1
        
        return result
    
    def add(self, module: str, message: str, excerpt: text.Excerpt = None):
        value = (module, message, excerpt) if excerpt else (module, message)
        self.values.append(value)
    
    def clear(self):
        self.values = []

    def has_value(self, message: str, position: tuple = None) -> bool:

        for value in self.values:

            if len(value) == 2:
                _, _message = value
                if _message == message:
                    return True
        
            else:
                _, _message, excerpt = value
                line, column = position
                if (_message == message 
                        and line == excerpt.position.line
                        and column == excerpt.position.column):
                    return True
        
        return False