from __future__ import annotations

import text
from text import helpers


def parse_integer(buffer: text.Buffer) -> str:

    character = buffer.read()
    if not helpers.in_range(character, '0', '9'):
        raise ValueError('expected a digit')

    result = ''
    while True:

        if buffer.finished():
            break
    
        if not helpers.in_range(character, '0', '9'):
            break
    
        result += buffer.read(consume=True)
        character = buffer.read()
    
    return result