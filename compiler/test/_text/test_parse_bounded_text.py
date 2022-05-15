from pytest import raises

from text import ParseBuffer, ErrorBuffer, parse_bounded_text
from .._grammar.helpers import run_valid_parse_test, run_invalid_parse_test


def test_simple():
    buffer = ParseBuffer('{test}')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')

    assert result == 'test'
    assert buffer.finished()


def test_nested():
    buffer = ParseBuffer('{{test}}')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')

    assert result == '{test}'
    assert buffer.finished()


def test_multi_character_string():
    buffer = ParseBuffer('### text ###')
    result = parse_bounded_text(buffer, ErrorBuffer(), '###')

    assert result == 'text'
    assert buffer.finished()


def test_invalid_sequence():
    buffer = ParseBuffer('{')
    result = parse_bounded_text(buffer, ErrorBuffer(), '{', '}')
    assert not result


def test_permit_newlines():
    arguments = {
        'start_sequence': '{',
        'end_sequence': '}',
        'permit_newlines': False
    }
    run_invalid_parse_test('{\n}', 
            parse_bounded_text, 
            [('unexpected newline', (1, -1))],
            arguments)


def test_unexpected_eof():
    arguments = {
        'start_sequence': '{',
        'end_sequence': '}',
        'permit_newlines': False
    }
    run_invalid_parse_test('{ ', 
            parse_bounded_text, 
            [('expected \'}\', got end-of-file', (1, -1))],
            arguments)

    arguments = {
        'start_sequence': '###',
        'permit_newlines': False
    }
    run_invalid_parse_test('### ', 
            parse_bounded_text, 
            [('expected \'###\', got end-of-file', (1, -1))],
            arguments)

