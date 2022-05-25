_colour_codes = {
    'black': 30,
    'red': 31,
    'green': 32,
    'yellow': 33,
    'blue': 34,
    'magenta': 35,
    'cyan': 36,
    'white': 37,
}


def colour(text: str, colour: str, bright: bool = False) -> str:

    global _colour_codes

    if colour not in _colour_codes:
        raise ValueError(f'no such colour \'{colour}\'')
    
    bright_flag = ';1' if bright else ''

    index = _colour_codes[colour]
    code = f'\033[{str(index)}{bright_flag}m'

    return f'{code}{text}\033[0m'
