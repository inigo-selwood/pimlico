import grammar
from utilities import test

def test_parse_valid():
    bounds = [
        '+',
        '{2}',
        '{2:}',
        '{2:3}',
    ]

    for predicate in ['!', '&']:
        for bound in bounds:

            text = f'{predicate}term{bound}'
            term = test.parse_valid(text, grammar.Term.parse)
            assert term.__str__() == text

def test_parse_valid():
    bounds = [
        '?',
        '*',
        '{:2}',
    ]

    for predicate in ['!', '&']:
        for bound in bounds:

            text = f'{predicate}term{bound}'
            errors = test.parse_invalid(text, grammar.Term.parse)
            assert errors.has_value('predicate given for an optional term',
                    position=(1, 1))

def test_match():

    # Predicates
    # And predicate
    term = test.parse_valid('!\'.\'', grammar.Term.parse)
    test.match_valid(term, ',')
    test.match_invalid(term, '.')

    # And predicate
    term = test.parse_valid('&\'.\'', grammar.Term.parse)
    test.match_valid(term, '.')
    test.match_invalid(term, ',')

    # Term bounds
    # Zero or more
    term = test.parse_valid('\'.\'*', grammar.Term.parse)
    test.match_valid(term, ' ')
    test.match_valid(term, '.')
    test.match_valid(term, '..')

    # Optional
    term = test.parse_valid('\'.\'?', grammar.Term.parse)
    test.match_valid(term, ' ')
    test.match_valid(term, '.')

    # One or more
    term = test.parse_valid('\'.\'+', grammar.Term.parse)
    test.match_valid(term, '.')
    test.match_valid(term, '..')

    # N instances
    term = test.parse_valid('\'.\'{2}', grammar.Term.parse)
    test.match_invalid(term, '.')
    test.match_valid(term, '..')

    # Up to N instances
    term = test.parse_valid('\'.\'{:2}', grammar.Term.parse)
    test.match_valid(term, '.')
    test.match_valid(term, '..')

    # N or more instances
    term = test.parse_valid('\'.\'{2:}', grammar.Term.parse)
    test.match_invalid(term, '.')
    test.match_valid(term, '..')
    test.match_valid(term, '...')

    # Between N and M instances
    term = test.parse_valid('\'.\'{2:3}', grammar.Term.parse)
    test.match_invalid(term, '.')
    test.match_valid(term, '..')
    test.match_valid(term, '...')