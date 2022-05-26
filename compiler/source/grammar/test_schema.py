import grammar
from utilities import test


def test_simple():

    # Single rule
    text = 'rule0 := \'term0\''
    schema = test.parse_valid(text, grammar.Schema.parse)
    assert text == schema.__str__()

    # Mutliple rules
    text = ('rule0 := \'term0\''
            '\n'
            '\nrule1 := \'term1\'')
    schema = test.parse_valid(text, grammar.Schema.parse)
    assert text == schema.__str__()

    # Inclusion macro
    text = '.include(string)'
    schema = test.parse_valid(text, grammar.Schema.parse)
    assert text == schema.__str__()

    # Multi-choice rule
    text = ('rule0 :='
            '\n    - \'term0\' {{'
            '\n        // Some C++'
            '\n    }}'
            '\n    - \'term1\' {{'
            '\n        // Some more C++'
            '\n    }}'
            '\n'
            '\nrule1 := \'term2\' {{'
            '\n        // Even more C++'
            '\n    }}')
    schema = test.parse_valid(text, grammar.Schema.parse)
    assert text == schema.__str__()


def test_empty_text():

    # Empty grammar
    errors = test.parse_invalid(' ', grammar.Schema.parse)
    assert errors.has_value('empty text')

    # Invalid indentation
    errors = test.parse_invalid('    rule := \'term\'', grammar.Schema.parse)
    assert errors.has_value('invalid indentation', position=(1, 5))

    # No rule
    errors = test.parse_invalid('{}', grammar.Schema.parse)
    assert errors.has_value('expected a rule', position=(1, 1))

    # Duplicate rule
    text = ('rule := \'term\''
            '\nrule := \'otherTerm\'')
    errors = test.parse_invalid(text, grammar.Schema.parse)
    assert errors.has_value('duplicate rule', position=(2, 1))
    assert errors.has_value('first defined here', position=(1, 1))

    # Trailing garbage
    text = 'rule := \'term\' {{ // Some C++ }} {}'
    errors = test.parse_invalid(text, grammar.Schema.parse)
    assert errors.has_value('expected newline', position=(1, 34))

    # errors = test.parse_invalid('rule := reference', grammar.Schema.parse, errors) 
    # assert errors.has_value('undefined reference', position=(1, 9))