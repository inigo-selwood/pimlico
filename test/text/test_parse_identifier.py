from pytest import raises

from text import ParseBuffer, parse_identifier


def test_simple():
    buffer = ParseBuffer('test ')
    assert parse_identifier(buffer) == 'test'
    assert not buffer.finished()


def test_set():
    text = 'azAZ09_'
    buffer = ParseBuffer(text)
    assert parse_identifier(buffer) == text
    assert buffer.finished()


def test_invalid():
    with raises(ValueError):
        parse_identifier(ParseBuffer('.test'))