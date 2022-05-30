import os

import tools
import text
import grammar
import utilities


def generate(flags: dict, parameters: dict, errors: tools.ErrorLog) -> bool:
    
    # Check the file exists
    grammar_file = parameters['grammar-file']
    grammar_text = utilities.read_file(grammar_file, errors)
    
    # Generate a schema
    buffer = text.Buffer(grammar_text)
    schema = grammar.Schema.parse(buffer, errors)
    if schema is None:
        return False
    
    print(schema)

    return True