from __future__ import annotations

import text
from text import helpers
import tools


def parse_expression(buffer: text.Buffer, 
        tokens: tuple, 
        errors: tools.ErrorLog,
        permit_newlines: bool = False,
        escape_codes: bool = False):
    
    start_token, end_token = tokens

    if not buffer.match(start_token, consume=True):
        raise ValueError(f'expected \'{start_token}\'')

    stack = 1

    value = start_token
    while True:

        # Check buffer not finished
        if buffer.finished():
            errors.add(__name__, 'unexpected end-of-file', buffer.excerpt())
            return None
        
        # Check not newline, unless permitted
        elif not permit_newlines and buffer.match('\n'):
            errors.add(__name__, 'unexpected newline', buffer.excerpt())
            return None
        
        # Allow escaped end-sequence
        elif escape_codes and buffer.match(f'\\{end_token}', consume=True):
            value += end_token
        
        # Allow other escape codes
        elif escape_codes and buffer.match('\\'):
            code = helpers.parse_escape_code(buffer, errors)
            if code is None:
                return None
            value += code
        
        # Check for end token
        elif buffer.match(end_token, consume=True):
            value += end_token
        
            stack -= 1
            if not stack:
                break
        
        # Check for start token
        elif buffer.match(start_token, consume=True):
            value += start_token
            stack += 1
        
        # Otherwise, add to value
        else:
            value += buffer.read(consume=True)
    
    return value