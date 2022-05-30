import grammar
from utilities import test


def test_parse_valid():

    # Simple
    production = test.parse_valid('term', grammar.Production.parse)
    assert production.__str__() == 'term'

    # With an embedded expression
    text = ('term {{'
            '\n        // Some C++'
            '\n    }}')
    production =  test.parse_valid(text, grammar.Production.parse)
    assert production.__str__() == text
    
    # Sequence, and expression
    text = ('termA termB {{'
            '\n        // Some C++'
            '\n    }}')
    production =  test.parse_valid(text, grammar.Production.parse)
    assert production.__str__() == text

    # Root-level enclosed seqeunce
    text = ('(termA termB) | termC {{'
            '\n        // Some C++'
            '\n    }}')
    production =  test.parse_valid(text, grammar.Production.parse)
    assert production.__str__() == text
    
    # Choice, and an expression
    text = ('termA | termB {{'
            '\n        // Some C++'
            '\n    }}')
    production =  test.parse_valid(text, grammar.Production.parse)
    assert production.__str__() == text

    # Expression with nested brackets in it
    text = ('term {{'
            '\n        int main() {'
            '\n            return 0;'
            '\n        }'
            '\n    }}')
    production = test.parse_valid(text, grammar.Production.parse)
    assert production.__str__() == text


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


def test_match():

    # Sequence
    production = test.parse_valid('\'.\'\',\'', grammar.Production.parse)
    test.match_valid(production, '.,')
    test.match_invalid(production, '.')

    # Choice
    production = test.parse_valid('\'.\' | \',\'', grammar.Production.parse)
    test.match_valid(production, '.')
    test.match_valid(production, ',')
    test.match_invalid(production, ';')
