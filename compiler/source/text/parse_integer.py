from utilities import in_range

from .parse_buffer import ParseBuffer


def parse_integer(buffer: ParseBuffer) -> int:
    ''' Parses an integer

    Arguments
    ---------
    buffer: ParseBuffer
        buffer containing integer
    
    Returns
    -------
    identifier: str
        the parsed integer
    
    Raises
    ------
    none: ValueError
        if no integer was found
    '''
    
    text = ''
    while True:

        character = buffer.read()
        if buffer.finished() or not in_range(character, '0', '9'):
            break
        
        text += buffer.read(True)

    if not text:
        raise ValueError('no integer present')

    return int(text)