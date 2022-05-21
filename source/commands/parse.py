import tools

from .help import help


def parse(arguments: list, errors: tools.ErrorLog) -> bool:

    # Split arguments into flags, parameters
    flags = []
    parameters = []

    for argument in arguments[1:]:
        if len(argument) > 1 and argument[:2] == '--':
            flags.append(argument)
        else:
            parameters.append(argument)
    
    pair = (flags, parameters)
    
    # Handle help flag
    if flags and flags[0] == '--help':
        return help(pair)

    # Handle subcommands
    elif not parameters:
        errors.add(__name__, 'expected a command')
        return False
    
    commands = {}
    
    command = parameters[0]

    if command not in commands:
        errors.add(__name__, f'unrecognized command \'{command}\'')
    
    return commands[command](pair)