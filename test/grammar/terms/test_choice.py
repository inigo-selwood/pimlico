from text import ParseBuffer, ErrorBuffer

from grammar.terms import Choice


def test_choice():
    buffer = ParseBuffer('\'a\' | [bc] | d | (e f) | `gh`')
    choice = Choice.parse(buffer, ErrorBuffer())
    assert choice
    assert choice.type == 'choice'
    assert buffer.finished()


def test_duplicate():
    errors = ErrorBuffer()
    buffer = ParseBuffer('a | a | b')
    assert not Choice.parse(buffer, errors)
    assert buffer.finished()
    
    _, message, position = errors.instances[0]
    assert message == 'duplicate option'
    assert (position.line, position.column) == (1, 5)


def test_unexpected_end_of_file():
    errors = ErrorBuffer()
    assert not Choice.parse(ParseBuffer('a |'), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-file'
    assert (position.line, position.column) == (1, -1)


def test_unexpected_end_of_line():
    errors = ErrorBuffer()
    assert not Choice.parse(ParseBuffer('a |\n'), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-line'
    assert (position.line, position.column) == (1, -1)


def test_empty_option():
    errors = ErrorBuffer()
    assert not Choice.parse(ParseBuffer('a ||'), errors)

    _, message, position = errors.instances[0]
    assert message == 'empty option'
    assert (position.line, position.column) == (1, 3)