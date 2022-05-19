from text import ParseBuffer, ErrorBuffer
from grammar import Term


def test_simple():

    text = '(a | b)'
    assert Term.parse(ParseBuffer(text), ErrorBuffer()).__str__() == 'a | b'

    text = '(a b)'
    assert Term.parse(ParseBuffer(text), ErrorBuffer()).__str__() == 'a b'

    texts = [
        '\'a\'',
        '[az]',
        'a',
        '`az`'
    ]

    for text in texts:
        assert Term.parse(ParseBuffer(text), ErrorBuffer()).__str__() == text


def test_bindings():
    texts = [
        'binding: (a | b)',
        'binding: \'a\'',
        'binding: [az]',
        'binding: a',
        'binding: (a b)',
        'binding: `az`'
    ]

    for text in texts:
        term = Term.parse(ParseBuffer(text), ErrorBuffer())
        assert term.__str__() == text
        assert term.binding == 'binding'


def test_escape_codes():

    codes = [
        r'\n',
        r'\r',
        r'\t',
        r'\v',
        r'\\',
        '\\\'',
        r'\"',
    ]

    for code in codes:
        for text in [f'\'{code}\'', f'`.{code}`']:
            errors = ErrorBuffer()
            buffer = ParseBuffer(text)
            result = Term.parse(buffer, errors).__str__() 

            assert result == text, errors.__str__()

    texts = [
        '[\' \'~]',
        r'[[\]]',
        r'`.\``'
    ]

    for text in texts:
        errors = ErrorBuffer()
        buffer = ParseBuffer(text)
        assert Term.parse(buffer, errors).__str__() == text, errors.__str__()