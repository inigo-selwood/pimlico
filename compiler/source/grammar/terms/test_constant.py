from grammar import terms
from utilities import test


def test_parse_valid():

    # Simple term
    constant = test.parse_valid('\' \'', terms.Constant.parse)
    assert constant.__str__() == '\' \''
    
    # All escape codes
    text = '\' \\\' \\\" \\\\ \\t \\v \\n \\r \''
    constant = test.parse_valid(text, terms.Constant.parse)
    assert constant.__str__() == text

def test_parse_invalid():

    # Unexpected EOF
    errors = test.parse_invalid('\'', terms.Constant.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    # Unexpected newline
    errors = test.parse_invalid('\'\n', terms.Constant.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))


def test_match():

    # Simple constant
    constant = test.parse_valid('\'test\'', terms.Constant.parse)
    match_text = test.match_valid(constant, 'test')
    assert match_text == 'test'

    test.match_invalid(constant, 'not test')

    # Escape codes
    codes = {
        '\\\'': '\'',
        '\\\"': '\"',
        '\\\\': '\\',
        '\\t': '\t',
        '\\v': '\v',
        '\\n': '\n',
        '\\r': '\r',
    }
    
    for code, value in codes.items():
        constant = test.parse_valid(f'\'{code}\'', terms.Constant.parse)
        match_text = test.match_valid(constant, value)
        assert match_text == value