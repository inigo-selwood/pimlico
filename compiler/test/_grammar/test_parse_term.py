from grammar import Term

from .helpers import run_invalid_parse_test, run_valid_parse_test


def test_instance_bounds():
    bounds = [
        'term+', 
        'term*', 
        'term?',
        'term{2}',
        'term{:2}',
        'term{2:}',
        'term{1:2}',
    ]
    for text in bounds:
        run_valid_parse_test(text, Term.parse)


def test_binding():
    texts = [
        'choice: (a | b)',
        'constant: \'a\'',
        'range: [az]',
        'reference: a',
        'sequence: (a b)',
        'set: `az`',
    ]

    for text in texts:
        run_valid_parse_test(text, Term.parse)


def test_n_instance_errors():
    errors = [('zero-valued bound', (1, 5))]
    run_invalid_parse_test('term{0}', Term.parse, errors)
    
    errors = [('redundant bound', (1, 5))]
    run_invalid_parse_test('term{1}', Term.parse, errors)


def test_n_or_more_instance_errors():
    errors = [('improper bound (should be \'*\')', (1, 5))]
    run_invalid_parse_test('term{0:}', Term.parse, errors)
    
    errors = [('improper bound (should be \'+\')', (1, 5))]
    run_invalid_parse_test('term{1:}', Term.parse, errors)

def test_up_to_n_instance_errors():
    errors = [('zero-valued bound', (1, 5))]
    run_invalid_parse_test('term{:0}', Term.parse, errors)
    
    errors = [('improper bound (should be \'?\')', (1, 5))]
    run_invalid_parse_test('term{:1}', Term.parse, errors)


def test_between_n_m_instance_errors():
    errors = [('improper bounds (should be \'{2}\')', (1, 5))]
    run_invalid_parse_test('term{2:2}', Term.parse, errors)

    errors = [('invalid bounds (2 > 1)', (1, 5))]
    run_invalid_parse_test('term{2:1}', Term.parse, errors)

    errors = [('improper bounds (should be \'{:2}\')', (1, 5))]
    run_invalid_parse_test('term{0:2}', Term.parse, errors)

