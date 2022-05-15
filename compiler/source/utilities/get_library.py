from ctypes import cdll

from .get_root import get_root


_library = None


def get_library() -> cdll:
    ''' Loads the compiled Pimlico shared C library

    Note: requires that PIMLICO_ROOT be set

    Returns
    -------
    library: cdll
        the loaded shared library
    '''

    global _library
    
    if not _library:

        root = get_root()
        path = f'{root}/library/library.so'
        if not os.path.exists(path):
            raise Exception(f'library.so not found at {path}')
        
        _library = cdll.LoadLibrary(path)
    
    return _library