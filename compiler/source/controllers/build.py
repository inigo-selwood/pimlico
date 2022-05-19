import os

from grammar import Program
from text import ParseBuffer, ErrorBuffer


def build(grammar_file: str, errors: ErrorBuffer) -> Program:

    section = 'controllers:build'

    if not os.path.exists(grammar_file):
        errors.add(section, f'could not open \'{grammar_file}\'')
        return None
    
    # Read contents of grammar file
    text = ''
    with open(grammar_file, 'r') as file:
        text = file.read().strip()

    # Check text isn't empty
    if not text:
        errors.add(section, 'empty grammar')
        return None
    
    # Do the parsings
    buffer = ParseBuffer(text, comment_sequence='#')
    program = Program.parse(buffer, errors)
    if not program:
        return None
    
    return program
