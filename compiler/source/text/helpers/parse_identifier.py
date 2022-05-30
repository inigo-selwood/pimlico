from __future__ import annotations

import text
from text import helpers


def parse_identifier(buffer: text.Buffer) -> str:    
    
    result = ''
    while True:

        if buffer.finished():
            break
    
        character = buffer.read()
        if (character != '_'
                and not helpers.in_range(character, 'a', 'z')
                and not helpers.in_range(character, 'A', 'Z')
                and not helpers.in_range(character, '0', '9')):
            break
    
        result += buffer.read(consume=True)
    
    if result == '' or helpers.in_range(result[0], '0', '9'):
        raise ValueError('expected an underscore or letter')

    return result