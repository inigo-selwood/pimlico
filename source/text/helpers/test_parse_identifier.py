import pytest
import text
from text import helpers
import tools


def test_valid():

    # All digits
    buffer = text.ParseBuffer('_azAZ09')
    result = helpers.parse_identifier(buffer)
    assert result == '_azAZ09'

    # Character terminated
    buffer = text.ParseBuffer('identifier ')
    result = helpers.parse_identifier(buffer)
    assert result == 'identifier'
    

def test_invalid():    
    
    # Leading digit / no identifier present
    with pytest.raises(ValueError):
        buffer = text.ParseBuffer('09_azAZ')
        helpers.parse_identifier(buffer)