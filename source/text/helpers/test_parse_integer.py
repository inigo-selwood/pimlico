import pytest
import text
from text import helpers
import tools


def test_valid():
    
    # All digits
    buffer = text.ParseBuffer('0123456789')
    result = helpers.parse_integer(buffer)
    assert result == '0123456789'
    
    # Character terminated
    buffer = text.ParseBuffer('1024 ')
    result = helpers.parse_integer(buffer)
    assert result == '1024'


def test_invalid():    
    
    # Leading digit / no identifier present
    with pytest.raises(ValueError):
        buffer = text.ParseBuffer(' ')
        helpers.parse_integer(buffer)