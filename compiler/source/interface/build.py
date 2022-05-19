import os

import controllers
from grammar import Program
from text import ParseBuffer, ErrorBuffer


def build(parameters: tuple, errors: ErrorBuffer) -> bool:
    ''' Builds a Pimlico library and header file

    Arguments
    ---------
    parameters: tuple
        (flags, arguments) lists
    errors: ErrorBuffer
        for reporting errors
    
    Returns
    -------
    success: bool
        True if the building was successful
    '''

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
    
    program = controllers.build(grammar_file, errors)

    return True
