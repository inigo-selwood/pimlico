def requires_escaping(character: str) -> bool:
    characters = [
        '\t',
        '\v',
        '\r',
        '\n',
        '\\',
        '\'',
        '\"',
    ]

    return character in characters