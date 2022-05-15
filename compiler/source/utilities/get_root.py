import os

_root = None


def get_root() -> str:
    ''' Loads the compiled Pimlico shared C library

    Note: requires that PIMLICO_ROOT be set

    Returns
    -------
    library: cdll
        the loaded shared library
    '''

    global _root
    
    if not _root:

        _root = os.environ.get('PIMLICO_ROOT')
        if not _root:
            raise Exception('PIMLICO_ROOT not set')
    
    return _root