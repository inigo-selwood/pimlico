import os

from text import ParseBuffer, ErrorBuffer
from grammar import Program


def run_build(parameters: tuple, errors: ErrorBuffer) -> bool:
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
    if not os.path.exists(grammar_file):
        errors.add(section, f'could not open \'{grammar_file}\'')
        return False
    
    # Read contents of grammar file
    text = ''
    with open(grammar_file, 'r') as file:
        text = file.read().strip()

    # Check text isn't empty
    if not text:
        errors.add(section, 'empty grammar')
        return False
    
    # Do the parsings
    buffer = ParseBuffer(text, comment_sequence='#')
    if not Program.parse(buffer, errors):
        return False
    
    return True
