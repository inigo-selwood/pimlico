from pytest import raises

from utilities import escape_code


def test_escape_codes():
    characters = {
        '\t': r'\t',
        '\v': r'\v',
        '\r': r'\r',
        '\n': r'\n',
        '\\': r'\\',
        '\'': '\\\'',
        '"': r'\"',
    }

    for character, value in characters.items():
        assert escape_code(character) == value