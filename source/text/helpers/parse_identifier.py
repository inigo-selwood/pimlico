from __future__ import annotations

import text
from text import helpers


def parse_identifier(buffer: text.ParseBuffer) -> str:    
    
    character = buffer.read()
    if not (character == '_'
            or helpers.in_range(character, 'a', 'z')
            or helpers.in_range(character, 'A', 'Z')):
        raise ValueError('expected an underscore or letter')

    result = ''
    while True:

        if buffer.finished():
            break
    
        if (character != '_'
                and not helpers.in_range(character, 'a', 'z')
                and not helpers.in_range(character, 'A', 'Z')
                and not helpers.in_range(character, '0', '9')):
            break
    
        result += buffer.read(consume=True)
        character = buffer.read()
    
    return result