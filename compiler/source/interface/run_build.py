import os

from text import ParseBuffer, ErrorBuffer
from grammar import Program


def run_build(parameters, errors: ErrorBuffer) -> bool:

    section = 'interface:build'

    flags, arguments = parameters
    assert arguments[0] == 'build'

    if len(arguments) < 2:
        errors.add(section, 'argument \'grammar\' missing')
        return False
    
    grammar_file = arguments[1]
    if not os.path.exists(grammar_file):
        errors.add(section, 'could not open \'{grammar_file}\'')
        return False
    
    text = ''
    with open(grammar_file, 'r') as file:
        text = file.read().strip()

    if not text:
        errors.add(section, 'empty grammar')
        return False
    
    buffer = ParseBuffer(text)

    if not Program.parse(buffer, errors):
        return False
    
    return True
