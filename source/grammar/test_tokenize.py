import tools
import grammar


def test_valid():

    tokens = {
        '\n': ['newline'],
        '\n\t': ['newline', 'indent'],
        '\n    ': ['newline', 'indent'],
        '\n    \n': ['newline', 'indent', 'newline'],

        '# comment': [('comment', 'comment')],
        '# comment\n': [('comment', 'comment'), 'newline'],

        '{{ }}': [('expression', '')],

        '\' \'': [('constant', ' ')],

        '`ab`': [('set', 'ab')],

        '_azAZ09': [('identifier', '_azAZ09')],

        '0': [('integer', '0')],
        '1024': [('integer', '1024')],
    }

    for token, value in tokens.items():
        errors = tools.ErrorLog()
        result = grammar.tokenize(token, errors)

        errors = errors.__str__()
        detail = errors if errors else token
        assert result == value, detail

    constants = [
        ':=', '-',
        '|', # Choices
        '(', ')', # Sequences
        '[', ']', # Ranges
        '{', ':', '}', # Specific bounds
        '+', '*', '?', # Shorthand bounds
        '!', '&' # Predicates
    ]

    for constant in constants:
        assert grammar.tokenize(constant, errors) == [constant]


def test_invalid():
    errors = tools.ErrorLog()

    # Invalid indentation
    assert not grammar.tokenize(' ', errors)
    assert errors.has_value('invalid indentation', (1, -1))

    # Invalid expression
    assert not grammar.tokenize('{{', errors)

    # Invalid constant
    assert not grammar.tokenize('\'', errors)
    
    # Empty constant
    errors.clear()
    assert not grammar.tokenize('\'\'', errors)
    assert errors.has_value('empty constant', (1, 1))

    # Invalid set
    assert not grammar.tokenize('`', errors)
    
    # Empty set
    errors.clear()
    assert not grammar.tokenize('``', errors)
    assert errors.has_value('too few characters in set', (1, 1))
    
    # No token
    errors.clear()
    assert not grammar.tokenize('@', errors)
    assert errors.has_value('expected a token', (1, 1))