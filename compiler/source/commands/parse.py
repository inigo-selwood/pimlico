from __future__ import annotations

import tools
import commands


def parse(arguments: list, errors: tools.ErrorLog) -> bool:

    arguments = arguments[1:]

    # # Split arguments into flags, parameters
    # flags = []
    # parameters = []

    # for argument in arguments[1:]:
    #     if len(argument) > 1 and argument[:2] == '--':
    #         flags.append(argument)
    #     else:
    #         parameters.append(argument)
    
    # pair = (flags, parameters)
    
    if not len(arguments):
        errors.add(__name__, 'expected arguments')
        return False

    # Handle help flag
    elif arguments[0] == '--help':
        return commands.help(arguments)
    
    command = arguments[0]

    if len(command) > 2 and command[:2] == '--':
        errors.add(__name__, f'expected a command, not \'{command}\'')
        return False

    bindings = {
        'generate': commands.generate,
    }

    if command not in bindings:
        errors.add(__name__, f'unrecognized command \'{command}\'')
    
    return bindings[command](arguments, errors)