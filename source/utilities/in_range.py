from argparse import ArgumentError


def in_range(character: str, lower: str, upper: str) -> bool:
    ''' Checks whether a character lies between two others

    Arguments
    ---------
    character: str
        the character to check if is in-range
    lower: str
        character must be greater than or equal to this
    upper: str
        character must be less than or equal to this
    
    Returns
    -------
    in_range: bool
        true if the character is in range
    '''
    
    if (len(character) != 1 
            or len(lower) != 1
            or len(upper) != 1):
        raise ValueError('parameters must be strings of length 1')
    
    character = ord(character)
    lower = ord(lower)
    upper = ord(upper)

    return character >= lower and character <= upper