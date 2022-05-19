from controllers import build
from text import ErrorBuffer


def map(grammar_file: str, errors: ErrorBuffer):

    program = build(grammar_file, errors)
    if not program:
        return None
    
    return None