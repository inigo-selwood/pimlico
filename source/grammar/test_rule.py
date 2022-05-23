import grammar
from utilities import test


def test_parse_valid():

    # Simple term
    test.parse_valid('rule := term', grammar.Rule.parse)

    # Rule with embedded expression
    test.parse_valid('rule := term {{ // Some C++ }}', grammar.Rule.parse)

    # Typed rule
    test.parse_valid('rule <Type> := term', grammar.Rule.parse)

    # Typed rule with nested brackets
    text = 'rule <std::vector<std::string>> := term'
    test.parse_valid(text, grammar.Rule.parse)

    # Indented multi-line rule
    text = ('rule := '
            '\n    - term0'
            '\n    - term1')
    test.parse_valid(text, grammar.Rule.parse)

    # Multi-line rule with expressions
    text = ('rule := '
            '\n    - term0 {{'
            '\n        // Some C++'
            '\n    }}'
            '\n    - term1 {{'
            '\n        // Some more C++'
            '\n    }}')
    test.parse_valid(text, grammar.Rule.parse)


def test_parse_invalid():

    # No assign operator
    errors = test.parse_invalid('rule', grammar.Rule.parse)
    assert errors.has_value('expected \':=\'', (1, -1))

    # Missing production
    errors = test.parse_invalid('rule := {}', grammar.Rule.parse)
    assert errors.has_value('expected productions', (1, 9))

    # Missing term
    text = ('rule :='
            '\n    - someTerm {{ // Some C++ }}'
            '\n    - {{ // Some more C++}}')
    errors = test.parse_invalid(text, grammar.Rule.parse)
    assert errors.has_value('expected a term', (3, 7))

    # Invalid indentation
    text = ('rule :='
            '\n    - term0'
            '\n        - term1')
    errors = test.parse_invalid(text, grammar.Rule.parse)
    assert errors.has_value('invalid indentation', (3, 9))
