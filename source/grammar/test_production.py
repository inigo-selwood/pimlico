import grammar
from utilities import test


def test_parse_valid():

    # Simple
    test.parse_valid('term', grammar.Production.parse)

    # With an embedded expression
    test.parse_valid('term {{ // Some C++ }}', grammar.Production.parse)

    # Sequence, and expression
    test.parse_valid('a b {{ // Some C++ }}', grammar.Production.parse)

    # Root-level enclosed seqeunce
    test.parse_valid('(a | b) {{ // Some C++ }}', grammar.Production.parse)
    
    # Choice, and an expression
    test.parse_valid('a | b {{ // Some C++ }}', grammar.Production.parse)

    # Expression with nested brackets in it
    test.parse_valid('term {{ { // Some C++ } }}', grammar.Production.parse)


def test_parse_invalid():

    # Expected closing bracket
    errors = test.parse_invalid(' ', grammar.Production.parse)
    assert errors.has_value('expected a term', position=(1, 1))

    # Empty expression
    errors = test.parse_invalid('term {{}}', grammar.Production.parse)
    assert errors.has_value('empty expression', position=(1, 6))

    # Error in term
    errors = test.parse_invalid('\'\'', grammar.Production.parse)

    # Invalid expression
    errors = test.parse_invalid('term {{}', grammar.Production.parse)