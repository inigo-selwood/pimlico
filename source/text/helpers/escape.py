_codes = {
    '\'': '\\\'',
    '\"': '\\\"',
    '\\': '\\\\',

    '\t': '\\t',
    '\v': '\\v',
    '\n': '\\n',
    '\r': '\\r',
}


def escape(value: str, custom_codes: dict = {}):

    global _codes

    codes = {**_codes, **custom_codes}

    result = ''
    for letter in value:
        if letter in codes:
            result += codes[letter]
        else:
            result += letter
    
    return result