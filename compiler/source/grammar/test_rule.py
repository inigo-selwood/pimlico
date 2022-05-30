import grammar
from utilities import test


def test_parse_valid():

    # Simple term
    text = 'rule := term'
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text

    # Rule with embedded expression
    text = ('rule := term {{'
            '\n        // Some C++'
            '\n    }}')
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text

    # Typed rule
    text = 'rule <Type> := term'
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text

    # Typed rule with nested brackets
    text = 'rule <std::vector<std::string>> := term'
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text

    # Indented multi-line rule
    text = ('rule :='
            '\n    - term0'
            '\n    - term1')
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text

    # Multi-line rule with expressions
    text = ('rule :='
            '\n    - term0 {{'
            '\n        // Some C++'
            '\n    }}'
            '\n    - term1 {{'
            '\n        // Some more C++'
            '\n    }}')
    rule = test.parse_valid(text, grammar.Rule.parse)
    assert rule.__str__() == text


def test_parse_invalid():

    # Check redefinition of reserved names
    reserved_names = [
        '_',
        '__',
        '__character__',
        '__identifier__',
        '__integer__',
        '__newline__',
        '__number__',
    ]

    for name in reserved_names:
        errors = test.parse_invalid(f'{name} := \'term\'', grammar.Rule.parse)
        assert errors.has_value('redefinition of reserved name', position=(1, 1))

    # No assign operator
    errors = test.parse_invalid('rule', grammar.Rule.parse)
    assert errors.has_value('expected \':=\'', position=(1, -1))

    # Missing production
    errors = test.parse_invalid('rule := {}', grammar.Rule.parse)
    assert errors.has_value('expected productions', position=(1, 9))

    # Missing term
    text = ('rule :='
            '\n    - someTerm {{ // Some C++ }}'
            '\n    - {{ // Some more C++}}')
    errors = test.parse_invalid(text, grammar.Rule.parse)
    assert errors.has_value('expected a term', position=(3, 7))

    # Invalid production indentation
    text = ('rule :='
            '\n    - term0'
            '\n        - term1')
    errors = test.parse_invalid(text, grammar.Rule.parse)
    assert errors.has_value('invalid indentation', position=(3, 9))


def test_match():

    # Single term
    rule = test.parse_valid('__root__ := \'.\'', grammar.Rule.parse)
    test.match_valid(rule, '.')

    # Choice
    text = ('__root__ := '
            '\n    - \'.\''
            '\n    - \',\'')
    rule = test.parse_valid(text, grammar.Rule.parse)
    test.match_valid(rule, '.')
    test.match_valid(rule, ',')