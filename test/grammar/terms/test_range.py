from text import ParseBuffer, ErrorBuffer

from grammar.terms import Range


def test_constant():
    grammars = [
        '[az]',
        '[ a z ]',
        '[09]',
        '[AZ]'
    ]

    for grammar in grammars:
        buffer = ParseBuffer(grammar)
        assert Range.parse(buffer, ErrorBuffer()), grammar
        assert buffer.finished(), grammar


def test_escaped_bracket():
    assert Range.parse(ParseBuffer(r'[[\]]'), ErrorBuffer())


def test_space():
    assert Range.parse(ParseBuffer('[\' \'~]'), ErrorBuffer())


def test_end_of_file():
    grammars = [
        '[',
        '[a',
        '[az'
    ]

    for grammar in grammars:
        errors = ErrorBuffer()
        assert not Range.parse(ParseBuffer(grammar), errors), grammar

        _, message, position = errors.instances[0]
        assert message == 'unexpected end-of-file'
        assert (position.line, position.column) == (1, -1)


def test_end_of_line():
    grammars = [
        '[\n',
        '[a\n',
        '[az\n'
    ]

    for grammar in grammars:
        errors = ErrorBuffer()
        assert not Range.parse(ParseBuffer(grammar), errors)

        _, message, position = errors.instances[0]
        assert message == 'unexpected end-of-line'
        assert (position.line, position.column) == (1, -1)


def test_too_few_characters():
    grammars = [
        '[]',
        '[a]'
    ]

    for grammar in grammars:
        errors = ErrorBuffer()
        assert not Range.parse(ParseBuffer(grammar), errors)

        _, message, position = errors.instances[0]
        assert message == 'too few characters'
        assert (position.line, position.column) == (1, 1)


def test_invalid_character():
    errors = ErrorBuffer()
    assert not Range.parse(ParseBuffer('[\u0000a]'), errors)

    _, message, position = errors.instances[0]
    assert message == 'invalid character'
    assert (position.line, position.column) == (1, 2)