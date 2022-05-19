import sys

from text import ErrorBuffer

from .help import help
from .build import build
from .map import map


def parse(errors: ErrorBuffer) -> bool:

    section = 'interface:parse'

    # Split arguments into flags and (positional) arguments
    flags = []
    arguments = []
    for value in sys.argv[1:]:
        if len(value) > 2 and value[:2] == '--':
            flags.append(value)
        else:
            arguments.append(value)
    
    # Help
    parameters = (flags, arguments)
    if '--help' in flags:
        return help(parameters, errors)
    
    # If not help, check for command
    if not arguments:
        errors.add(section, 'expected a command')
        return False

    # Delegate to command
    commands = {
        'build': build,
        'map': map
    }

    # Check command found
    command = arguments[0]
    if command not in commands:
        errors.add(section, f'unrecognized command \'{command}\'')
        return False
    
    return commands[command](parameters, errors)

    