import os

import controllers
from text import ParseBuffer, ErrorBuffer
from grammar import Program


def map(parameters: tuple, errors: ErrorBuffer) -> bool:

    section = 'interface:build'

    # Unpack flags and arguments
    _, arguments = parameters
    assert arguments[0] == 'build'

    # Check for a grammar file name argument
    if len(arguments) < 2:
        errors.add(section, 'argument \'grammar\' missing')
        return False
    
    # Make sure grammar file exists
    grammar_file = arguments[1]
    
    map = controllers.map(grammar_file)

    return True