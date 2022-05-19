import yaml

from text import ErrorBuffer
from utilities import get_configuration, get_root


def help(parameters: tuple, errors: ErrorBuffer) -> bool:
    ''' Prints tailored help messages

    Note: the messages themselves are written in a .yaml, pointed to by 
    the configuration object

    Arguments
    ---------
    parameters: tuple
        the (flags, arguments) pair
    errors: ErrorBuffer
        for error reporting
    
    Returns
    -------
    success: bool
        True if the desired help message was printed properly
    '''

    flags, arguments = parameters
    assert '--help' in flags

    section = 'interface:help'

    # Check there aren't too many flags/arguments
    if len(flags) > 1 or len(arguments) > 1:
        errors.add(section, 'too many flags/arguments')
        return False
    
    # Find the command, if there is one
    command = arguments[0] if arguments else 'pimlico'

    # Get path of help strings file
    root = get_root()
    path = get_configuration()['help-strings-path']
    path = f'{root}/{path}'

    # Load help strings
    help_strings = {}
    with open(path, 'r') as file:
        help_strings = yaml.safe_load(file.read())

    # Check one exists for the command given
    if command not in help_strings:
        errors.add(section, f'no help string for command \'{command}\'')
        return False

    print(help_strings[command])
    return True
