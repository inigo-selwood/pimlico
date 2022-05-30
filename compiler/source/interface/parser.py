from __future__ import annotations

import tools
import interface


class Parser:

    def __init__(self, detail: str):
        self.detail = detail
        self.commands = {}
    
    def add_command(self, command: interface.Command):
        self.commands[command.name] = command
    
    def usage(self) -> str:

        commands = ''
        index = 0
        count = len(self.commands)
        
        for name in self.commands:
            commands += name

            if index + 1 < count:
                commands += ', '
            index += 1

        return f'usage: pimlico [--help] {{{commands}}} ...'
    
    def help(self) -> str:

        assert self.commands

        commands = ''
        greatest_length = max([len(name) for name in self.commands])
        for name, command in self.commands.items():
            indent = ' ' * ((greatest_length - len(name)) + 4)
            commands += f'\n  {name}{indent}{command.detail}'

        return (f'{self.usage()}'
                '\n'
                f'\n{self.detail}'
                '\n'
                f'\ncommands:{commands}')
    
    def run(self, arguments: list, errors: tools.ErrorLog):

        # Discard first argument (file name)
        arguments = arguments[1:]
        
        if not len(arguments):
            print(self.usage())
            errors.add(__name__, 'expected arguments')
            return False

        # Handle help flag
        elif arguments[0] == '--help':
            
            # General help
            if len(arguments) == 1:
                print(self.help())
                return True
            
            # Help for a specific command
            else:

                if len(arguments) > 2:
                    print(self.usage())
                    errors.add(__name__, 'too many arguments for \'--help\'')
                    return False
                
                command = arguments[1]
                if command not in self.commands:
                    print(self.usage())
                    errors.add(__name__, f'no such command \'{command}\'')
                    return False
                
                print(self.commands[command].help())
                return True
            
        # Check there wasn't a flag in place of the command
        argument0 = arguments[0]
        if len(argument0) > 2 and argument0[:2] == '--':
            errors.add(__name__, f'unexpected flag \'{arguments[0]}\'')
            return False
        
        # Find the command
        command_name = argument0
        if command_name not in self.commands:
            print(self.usage())
            errors.add(__name__, f'unrecognized command \'{command_name}\'')
            return False

        command = self.commands[command_name]
        arguments = arguments[1:]
        
        # Parse flags and parameters
        flags = {}
        parameters = {}

        index = 0
        argument_count = len(arguments)
        while True:

            # Stop parsing if we've exhausted all the arguments
            if index == argument_count:
                break
            argument = arguments[index]

            # If it's a flag, parse it
            if len(argument) >= 2 and argument[:2] == '--':
                
                # Check there's text after the flag-indicating hyphens
                if len(argument) == 2:
                    errors.add(__name__, 'invalid flag \'--\'')
                    return False
                
                # Check the flag is in the list of flags supported by the 
                # command
                flag_name = argument[2:]
                if flag_name not in command.flags:
                    print(command.usage())
                    errors.add(__name__, f'unexpected flag \'{argument}\'')
                    return False
                flag = command.flags[flag_name]
                
                # For each parameter that the flag takes, add it to a dict
                flag_parameters = {}
                parameter_count = len(flag.parameters)
                while True:

                    # Stop if we've parsed the last parameter this flag takes
                    parameter_index = len(flag_parameters)
                    if parameter_index == parameter_count:
                        break
                
                    # Make sure we haven't run out of parameters when more were
                    # expected
                    index += 1
                    if index == argument_count:
                        print(command.usage())
                        errors.add(__name__, 
                                f'too few parameters for \'{flag_name}\'')
                        return False
                    argument = arguments[index]
                    
                    # Put the parameter in the dict
                    parameter_name = flag.parameters[parameter_index]
                    flag_parameters[parameter_name] = argument
                
                # Put the flag and its parameters in the dict
                flags[flag_name] = flag_parameters
        
            else:

                # Check the parameter was expected
                parameter_index = len(parameters)
                if parameter_index >= len(command.parameters):
                
                    print(command.usage())
                    errors.add(__name__, 
                            f'unexpected parameter \'{argument}\'')
                    return False
                
                # Add parameter to the dict
                parameter_name = command.parameters[parameter_index].name
                parameters[parameter_name] = argument
        
            index += 1
        
        # Check we got all the parameters we expected
        if len(parameters) < len(command.parameters):
            print(command.usage())
            errors.add(__name__, 'too few arguments')
            return None
        
        return command.functor(flags, parameters, errors)