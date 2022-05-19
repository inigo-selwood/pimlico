from utilities import in_range

from .parse_buffer import ParseBuffer


def parse_escape_code(buffer: ParseBuffer) -> str:
    ''' Parses an escape code

    Arguments
    ---------
    buffer: ParseBuffer
        buffer containing escape code
    
    Returns
    -------
    identifier: str
        the parsed escape code, or None if an error was encountered
    
    Raises
    ------
    none: ValueError
        if no escape code was found
    '''
    
    assert buffer.match('\\', consume=True)

    literals = ['\\', '\'', '\"']

    codes = {
        'n': '\n',
        'r': '\r',
        't': '\t',
        'v': '\v',
    }

    character = buffer.read()
    result = ''
    if character in literals:
        result = character
    elif character in codes:
        result = codes[character]
    else:
        return None
    
    buffer.increment()
    return result