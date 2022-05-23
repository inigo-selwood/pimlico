from grammar import terms
from utilities import test


def test_parse_valid():
    test.parse_valid('azAZ09_', terms.Reference.parse)