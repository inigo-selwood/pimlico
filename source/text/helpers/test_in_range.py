from text import helpers


def test_valid():
    assert helpers.in_range('a', 'a', 'b') # Start
    assert helpers.in_range('b', 'a', 'b') # End 
    assert helpers.in_range('b', 'a', 'c') # In-between


def test_invalid():
    assert not helpers.in_range('a', 'b', 'c') # Less-than
    assert not helpers.in_range('c', 'a', 'b') # More-than