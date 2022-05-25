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
