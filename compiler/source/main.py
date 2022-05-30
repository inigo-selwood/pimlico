import sys

import interface
import controllers
import tools


if __name__ == '__main__':

    parser = interface.Parser('a PEG parser generator for C/C++')

    # Add interactive command
    validate = interface.Command('validate', 
            'checks whether a given string belongs to a grammar',
            controllers.validate)
    validate.add_parameter('grammar-file', 
            'file to generate the parser from')
    validate.add_flag('input',
            'file to read input from',
            ['file'])
    parser.add_command(validate)

    # Add generate command
    generate = interface.Command('generate', 
            'creates a C/C++ parser header from a grammar file',
            controllers.generate)
    generate.add_parameter('grammar-file', 
            'file to generate the parser from')
    generate.add_flag('output', 
            'name of file to write output to', 
            ['file'])
    parser.add_command(generate)

    # Run program
    errors = tools.ErrorLog()
    success = parser.run(sys.argv, errors)
    
    if not success:
        print(errors)
        exit(1)