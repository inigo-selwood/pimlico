
def is_identifier_letter(letter):
    index = ord(letter)
    return ((index >= ord('a') and index <= ord('z'))
            or (index >= ord('A') and index <= ord('Z'))
            or letter == '_')
