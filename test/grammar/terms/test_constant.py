from text import ParseBuffer, ErrorBuffer

from grammar.terms import Constant


def test_constant():
    buffer = ParseBuffer('\'a\'')
    constant = Constant.parse(buffer, ErrorBuffer())
    assert constant
    assert constant.type == 'constant'
    assert buffer.finished()


def test_escaped_quote():
    buffer = ParseBuffer('\'\\\'\'')
    constant = Constant.parse(buffer, ErrorBuffer())

    assert constant
    assert buffer.finished()


def test_empty_constant():
    buffer = ParseBuffer('\'\'')
    errors = ErrorBuffer()
    assert not Constant.parse(buffer, errors)
    assert buffer.finished()

    _, message, position = errors.instances[0]
    assert message == 'empty'
    assert (position.line, position.column) == (1, 1)


def test_invalid_character():
    errors = ErrorBuffer()
    assert not Constant.parse(ParseBuffer('\'\t\''), errors)

    _, message, position = errors.instances[0]
    assert message == 'invalid character'
    assert (position.line, position.column) == (1, 2)


def test_end_of_line():
    errors = ErrorBuffer()
    assert not Constant.parse(ParseBuffer('\''), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-file'
    assert (position.line, position.column) == (1, -1)


def test_end_of_file():
    errors = ErrorBuffer()
    assert not Constant.parse(ParseBuffer('\'\n'), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-line'
    assert (position.line, position.column) == (1, -1)