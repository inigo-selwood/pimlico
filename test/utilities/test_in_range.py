from pytest import raises

from utilities import in_range


def test_in_range():
    assert in_range('a', 'a', 'z')
    assert in_range('z', 'a', 'z')


def test_not_in_range():
    assert not in_range('a', 'b', 'z')
    assert not in_range('z', 'a', 'y')


def test_bad_length():
    with raises(ValueError):
        in_range('', 'a', 'z')
    
    with raises(ValueError):
        in_range('aa', 'a', 'z')