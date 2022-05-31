from __future__ import annotations

import text
from text import helpers


def parse_in_set(buffer: text.Buffer, set: str) -> str:

    result = ''
    while True:

        character = buffer.read()
        if buffer.finished() or character not in set:
            break
    
        result += character
        buffer.increment()
    
    if not result:
        values = ', '.join(set)
        raise ValueError(f'expected a value in {{{values}}}')
    
    return result