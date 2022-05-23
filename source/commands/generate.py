import os

import tools
import text
import grammar


def generate(arguments: list, errors: tools.ErrorLog) -> bool:
    
    # Check this is a generate command
    if not arguments[0] == 'generate':
        raise ValueError('expected \'generate\' as first argument')
    
    # Make sure there's a grammar file
    elif len(arguments) < 2:
        errors.add(__name__, 'expected grammar file')
        return False
    
    # Check the file exists
    grammar_file = arguments[1]
    if not os.path.exists(grammar_file):
        errors.add(__name__, f'error opening file \'{grammar_file}\'')
        return False

    # Read the file's contents, check it isn't empty
    grammar_text = ''
    with open(grammar_file, 'r') as file:
        grammar_text = file.read()
    if not grammar_text:
        errors.add(__name__, 'empty grammar file')
        return False
    
    # Generate a schema
    buffer = text.Buffer(grammar_text)
    if grammar.Schema.parse(buffer, errors) is None:
        return False

    return True