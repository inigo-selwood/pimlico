from utilities import in_range

from .parse_buffer import ParseBuffer


def parse_identifier(buffer: ParseBuffer) -> str:
    ''' Parses an identifier (lowercase, uppercase, number, or underscore)

    Arguments
    ---------
    buffer: ParseBuffer
        buffer containing identifier
    
    Returns
    -------
    identifier: str
        the parsed identifier
    
    Raises
    ------
    none: ValueError
        if no identifier was found
    '''
    
    identifier = ''
    while True:

        character = buffer.read()
        if (buffer.finished()
                or (character != '_'
                    and not in_range(character, 'a', 'z')
                    and not in_range(character, 'A', 'Z')
                    and not in_range(character, '0', '9'))):
            break
        
        identifier += buffer.read(True)

    if not identifier:
        raise ValueError('no identifier present')

    return identifier