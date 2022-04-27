from argparse import ArgumentError


def in_range(character: str, lower: str, upper: str) -> bool:
    if (len(character) != 1 
            or len(lower) != 1
            or len(upper) != 1):
        raise ValueError('parameters must be strings of length 1')
    
    character = ord(character)
    lower = ord(lower)
    upper = ord(upper)

    return character >= lower and character <= upper