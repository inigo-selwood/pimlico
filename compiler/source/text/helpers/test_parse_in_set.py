import pytest
import text
from text import helpers
import tools


def test_valid():

    # Self-terminated parse
    buffer = text.Buffer('ab')
    result = helpers.parse_in_set(buffer, 'ab')
    assert result == 'ab'

    # Not-in-set terminated parse
    buffer = text.Buffer('ab ')
    result = helpers.parse_in_set(buffer, 'ab')
    assert result == 'ab'


def test_invalid():    
    
    # No value in set present
    with pytest.raises(ValueError):
        buffer = text.Buffer(' ')
        helpers.parse_in_set(buffer, 'ab')