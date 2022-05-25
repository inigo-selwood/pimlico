def in_range(character: str, lower: str, upper: str) -> bool:

    character = ord(character)
    lower = ord(lower)
    upper = ord(upper)

    return character >= lower and character <= upper