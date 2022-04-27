from pytest import raises

from utilities import in_range


def test_in_range():
    assert in_range('a', 'a', 'z')
    assert not in_range('a', 'b', 'z')

    with raises(ValueError):
        in_range('', 'a', 'z')