from grammar import terms
from utilities import test


def test_parse_valid():
    
    reference = test.parse_valid('azAZ09_', terms.Reference.parse)
    assert reference.__str__() == 'azAZ09_'