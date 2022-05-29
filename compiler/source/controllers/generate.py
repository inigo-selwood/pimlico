import os

import tools
import text
import grammar


def generate(flags: dict, parameters: dict, errors: tools.ErrorLog) -> bool:
    
    # Check the file exists
    grammar_file = parameters['grammar-file']
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
    schema = grammar.Schema.parse(buffer, errors)
    if schema is None:
        return False
    
    print(schema)

    return True