from grammar import terms
from utilities import test


def test_simple():
    
    test.parse_valid('`az`', terms.Set.parse)
    
    test.parse_valid('`+-*/%`', terms.Set.parse)
    
    test.parse_valid(r'`.\``', terms.Set.parse)

    codes = {
        '\\t': '\t',
        '\\n': '\n'
    }

    for sentence in ['`.\\t`', '`.\\n`']:
        test.parse_valid(sentence, terms.Set.parse)


def test_empty_set():

    errors = test.parse_invalid('``', terms.Set.parse)
    assert errors.has_value('empty', position=(1, 1))

    errors = test.parse_invalid('`a`', terms.Set.parse)
    assert errors.has_value('redundant (constant)', position=(1, 1))

    errors = test.parse_invalid('`', terms.Set.parse)
    assert errors.has_value('unexpected end-of-file', position=(1, -1))

    errors = test.parse_invalid('`\n', terms.Set.parse)
    assert errors.has_value('unexpected newline', position=(1, -1))
