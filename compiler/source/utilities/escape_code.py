def escape_code(character: str) -> str:
    characters = {
        '\t': '\\t',
        '\v': '\\v',
        '\r': '\\r',
        '\n': '\\n',
        '\\': '\\\\',
        '\'': '\\\'',
        '\"': '\\\"',
    }

    return characters[character]
