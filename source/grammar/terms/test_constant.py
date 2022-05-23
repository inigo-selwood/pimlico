from grammar import terms
from utilities import test


def test_parse_valid():

    test.parse_valid('\'a\'', terms.Constant.parse)
    
    test.parse_valid('\'\\\'\'', terms.Constant.parse)


def test_parse_invalid():

    # Unexpected EOF
    errors = test.parse_invalid('\'', terms.Constant.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    # Unexpected newline
    errors = test.parse_invalid('\'\n', terms.Constant.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))