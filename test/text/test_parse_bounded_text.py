from pytest import raises

from text import ParseBuffer, ErrorBuffer, parse_bounded_text


def test_simple():
    buffer = ParseBuffer('{test}')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')

    assert result == '{test}'
    assert buffer.finished()


def test_nested():
    buffer = ParseBuffer('{{test}}')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')

    assert result == '{{test}}'
    assert buffer.finished()


def test_multi_character_string():
    buffer = ParseBuffer('### text ###')
    result = parse_bounded_text(buffer, ErrorBuffer(), '###')

    assert result == '### text ###'
    assert buffer.finished()


def test_invalid_sequence():
    buffer = ParseBuffer('{')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')
    assert not result