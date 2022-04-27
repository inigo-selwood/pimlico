from text import ParseBuffer, ErrorBuffer

from grammar.terms import Set


def test_set():
    buffer = ParseBuffer('`ab`')
    assert Set.parse(buffer, ErrorBuffer())
    assert buffer.finished()


def test_escaped_backtick():
    assert Set.parse(ParseBuffer('`.\\``'), ErrorBuffer())


def test_escape_codes():
    codes = {
        '\\t': '\t',
        '\\n': '\n'
    }

    for code, value in codes.items():
        text = f'`.\\{code}`'
        buffer = ParseBuffer(text)
        set = Set.parse(buffer, ErrorBuffer())
        
        assert set
        assert value in set.values
        assert buffer.finished()


def test_empty_set():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('``'), errors)

    _, message, position = errors.instances[0]
    assert message == 'empty'
    assert (position.line, position.column) == (1, 1)


def test_redundant_constant():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('`a`'), errors)

    _, message, position = errors.instances[0]
    assert message == 'redundant (constant)'
    assert (position.line, position.column) == (1, 1)


def test_redundant_set():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('`abc`'), errors)

    _, message, position = errors.instances[0]
    assert message == 'redundant (range)'
    assert (position.line, position.column) == (1, 1)


def test_invalid_character():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('`\t`'), errors)

    _, message, position = errors.instances[0]
    assert message == 'invalid character'
    assert (position.line, position.column) == (1, 2)


def test_end_of_line():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('`'), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-file'
    assert (position.line, position.column) == (1, -1)


def test_end_of_file():
    errors = ErrorBuffer()
    assert not Set.parse(ParseBuffer('`\n'), errors)

    _, message, position = errors.instances[0]
    assert message == 'unexpected end-of-line'
    assert (position.line, position.column) == (1, -1)