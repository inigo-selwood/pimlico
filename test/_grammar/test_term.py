from grammar import Term

from .helpers import run_invalid_test, run_valid_test


def test_instance_bounds():
    for text in ['a+', 'a*', 'a?']:
        run_valid_test(text, Term.parse)


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
        run_valid_test(text, Term.parse)