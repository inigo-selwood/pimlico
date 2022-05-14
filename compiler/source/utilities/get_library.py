from ctypes import cdll

import os

_library = None


def get_library():
    ''' Loads the compiled Pimlico shared C library

    Note: requires that PIMLICO_ROOT be set

    Returns
    -------
    library: cdll
        the loaded shared library
    '''

    global _library
    
    if not _library:

        root = os.environ.get('PIMLICO_ROOT')
        if not root:
            raise Exception('PIMLICO_ROOT not set')
        
        path = f'{root}/library/library.so'
        if not os.path.exists(path):
            raise Exception(f'library.so not found at {path}')
        
        _library = cdll.LoadLibrary(path)
    
    return _library