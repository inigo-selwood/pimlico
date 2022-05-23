import pytest

import text
from text import helpers
import tools


def test_valid():
    errors = tools.ErrorLog()

    # Single value
    buffer = text.Buffer('{}')
    result = helpers.parse_expression(buffer, ('{', '}'), errors)
    assert result == '{}', errors.__str__()

    # Nested value
    buffer = text.Buffer('{{}}')
    result = helpers.parse_expression(buffer, ('{', '}'), errors)
    assert result == '{{}}', errors.__str__()

    # Multi-character tokens
    buffer = text.Buffer('{%%}')
    result = helpers.parse_expression(buffer, ('{%', '%}'), errors)
    assert result == '{%%}', errors.__str__()

    # Identical tokens
    buffer = text.Buffer('%%')
    result = helpers.parse_expression(buffer, ('%', '%'), errors)
    assert result == '%%', errors.__str__()

    # Value with newline
    buffer = text.Buffer('{\n}')
    result = helpers.parse_expression(buffer, 
            ('{', '}'), 
            errors, 
            permit_newlines=True)
    assert result == '{\n}', errors.__str__()

    # Escape codes
    buffer = text.Buffer('{\\\\ \\\' \\\" \\b \\t \\v \\r \\n}')
    result = helpers.parse_expression(buffer, 
            ('{', '}'), 
            errors, 
            escape_codes=True)
    assert result == '{\\ \' \" \b \t \v \r \n}', errors.__str__()

    # Escaped end sequence
    buffer = text.Buffer('{\\}}')
    result = helpers.parse_expression(buffer, 
            ('{', '}'), 
            errors, 
            escape_codes=True)
    assert result == '{}}', errors.__str__()


def test_invalid():
    errors = tools.ErrorLog()

    # Parser called when string not present
    with pytest.raises(ValueError):
        buffer = text.Buffer('{}')
        helpers.parse_expression(buffer, ('{%', '%}'), errors)

    # End-of-file
    errors.clear()
    buffer = text.Buffer('{')
    assert not helpers.parse_expression(buffer, ('{', '}'), errors)
    assert errors.has_value('unexpected end-of-file', (1, -1))

    # Newline when not permitted
    errors.clear()
    buffer = text.Buffer('{\n}')
    assert not helpers.parse_expression(buffer, ('{', '}'), errors)
    assert errors.has_value('unexpected newline', (1, -1))

    # Invalid escape code
    errors.clear()
    buffer = text.Buffer('{\\.}')
    assert not helpers.parse_expression(buffer, 
                ('{', '}'), 
                errors, 
                escape_codes=True)
    assert errors.has_value('invalid escape code', (1, 2))