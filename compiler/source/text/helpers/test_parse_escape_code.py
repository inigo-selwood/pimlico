import pytest
import text
from text import helpers
import tools


def test_valid():
    errors = tools.ErrorLog()

    codes = {
        '\\\\': '\\',
        '\\\'': '\'',
        '\\\"': '\"',
        '\\b': '\b',
        '\\t': '\t',
        '\\v': '\v',
        '\\r': '\r',
        '\\n': '\n',
    }

    for code, value in codes.items():

        # Single value
        buffer = text.Buffer(code)
        result = helpers.parse_escape_code(buffer, errors)
        assert result == value, errors.__str__()


def test_invalid():
    errors = tools.ErrorLog()

    # No code present
    with pytest.raises(ValueError):
        buffer = text.Buffer(' ')
        helpers.parse_escape_code(buffer, errors)
    
    # EOF in code
    errors.clear()
    buffer = text.Buffer('\\')
    assert not helpers.parse_escape_code(buffer, errors)
    assert errors.has_value('unexpected end-of-file', (1, -1)), errors.__str__()

    # Invalid
    errors.clear()
    buffer = text.Buffer('\\.')
    assert not helpers.parse_escape_code(buffer, errors)
    assert errors.has_value('invalid escape code', (1, 1))