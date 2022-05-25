import sys

import commands
import tools


if __name__ == '__main__':

    errors = tools.ErrorLog()
    success = commands.parse(sys.argv, errors)
    
    if not success:
        print(errors)
        exit(1)