import subprocess


def format_c(text: str):
    ''' Formats a payload of C/C++ text using clang-format

    Arguments
    ---------
    text: str
        the text to format
    
    Returns
    -------
    result: str
        the text with formatting applied (if possible, using clang-format), or
        the raw text if the program isn't installed
    '''

    result = ''

    try:
        response = subprocess.run(['clang-format'], 
                input=text.encode(), 
                stdout=subprocess.PIPE)

        if response.returncode != 0:
            result = text
        else:
            result = response.stdout.decode('utf-8')
    except FileNotFoundError:
        result = text
    
    return result
    