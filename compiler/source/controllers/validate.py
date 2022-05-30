import os

import tools
import text
import grammar
import utilities


def _read_input() -> str:
    lines = []

    print('>>> ', end='')
    while True:

        line = input()
        if not line:
            break
        
        lines.append(line)
        print('... ', end='')
    
    return '\n'.join(lines)


def validate(flags: dict, parameters: dict, errors: tools.ErrorLog) -> bool:
    
    # Check the file exists
    grammar_file = parameters['grammar-file']
    grammar_text = utilities.read_file(grammar_file, errors)
    if grammar_text is None:
        return False
    
    # Generate a schema
    buffer = text.Buffer(grammar_text)
    schema = grammar.Schema.parse(buffer, errors)
    if schema is None:
        return False
    
    print(schema)

    input_text = ''
    if 'input' in flags:
        input_file = flags['input']['file']
        input_text = utilities.read_file(input_file, errors)
        if input_text is None:
            return False
    
    else:
        input_text = _read_input()
    
    buffer = text.Buffer(input_text)
    print(schema.match(buffer))

    return True