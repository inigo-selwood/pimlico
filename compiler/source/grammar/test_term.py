import grammar
from utilities import test


def test_match():

    # Zero or more
    production = test.parse_valid('\'.\'*', grammar.Production.parse)
    test.match_valid(production, ' ')
    test.match_valid(production, '.')
    test.match_valid(production, '..')

    # Optional
    production = test.parse_valid('\'.\'?', grammar.Production.parse)
    test.match_valid(production, ' ')
    test.match_valid(production, '.')

    # One or more
    production = test.parse_valid('\'.\'+', grammar.Production.parse)
    test.match_valid(production, '.')
    test.match_valid(production, '..')

    # N instances
    production = test.parse_valid('\'.\'{2}', grammar.Production.parse)
    test.match_invalid(production, '.')
    test.match_valid(production, '..')

    # Up to N instances
    production = test.parse_valid('\'.\'{:2}', grammar.Production.parse)
    test.match_valid(production, '.')
    test.match_valid(production, '..')

    # N or more instances
    production = test.parse_valid('\'.\'{2:}', grammar.Production.parse)
    test.match_invalid(production, '.')
    test.match_valid(production, '..')
    test.match_valid(production, '...')

    # Between N and M instances
    production = test.parse_valid('\'.\'{2:3}', grammar.Production.parse)
    test.match_invalid(production, '.')
    test.match_valid(production, '..')
    test.match_valid(production, '...')