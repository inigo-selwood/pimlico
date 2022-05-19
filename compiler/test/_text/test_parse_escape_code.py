from pytest import raises

from text import ParseBuffer, parse_escape_code


def test_simple():
    escape_codes = {
        r'\n': '\n',
        r'\r': '\r',
        r'\t': '\t',
        r'\v': '\v',
        r'\\': '\\',
        '\\\'': '\'',
        r'\"': '"',
    }

    for code, value in escape_codes.items():
        buffer = ParseBuffer(code)
        assert parse_escape_code(buffer) == value
