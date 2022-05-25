from __future__ import annotations

from copy import copy

import text
import tools


def parse_escape_code(buffer: text.Buffer, errors: tools.ErrorLog) -> str:
    
    start_position = copy(buffer.position)
    if not buffer.match('\\', consume=True):
        raise ValueError('expected an escape code')

    if buffer.finished():
        errors.add(__name__, 'unexpected end-of-file', buffer.excerpt())
        return None
    
    character = buffer.read()
    if character in '\\\'\"':
        buffer.increment()
        return character
    
    codes = {
        'b': '\b',
        't': '\t',
        'v': '\v',
        'r': '\r',
        'n': '\n',
    }

    if character in codes:
        buffer.increment()
        return codes[character]
    
    errors.add(__name__, 
            f'invalid escape code', 
            buffer.excerpt(start_position))
    return None