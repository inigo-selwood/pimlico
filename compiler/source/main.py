import sys

import interface
import controllers
import tools


if __name__ == '__main__':

    parser = interface.Parser('a PEG parser generator for C/C++')

    # Add generate command
    generate_command = interface.Command('generate', 
            'creates a C/C++ parser header from a grammar file',
            controllers.generate)
    generate_command.add_parameter('grammar-file', 
            'file to generate the parser from')
    generate_command.add_flag('output', 
            'name of file to write output to', 
            ['file'])
    parser.add_command(generate_command)

    # Run program
    errors = tools.ErrorLog()
    success = parser.run(sys.argv, errors)
    
    if not success:
        print(errors)
        exit(1)