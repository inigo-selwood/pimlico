import os

import tools


def read_file(file_name: str, errors: tools.ErrorLog) -> str:
    
    # Check the file exists
    if not os.path.exists(file_name):
        errors.add(__name__, f'error opening file \'{file_name}\'')
        return None

    # Read the file's contents, check it isn't empty
    text = ''
    with open(file_name, 'r') as file:
        text = file.read()
    if not text:
        errors.add(__name__, 'file empty')
        return None
    
    return text
    