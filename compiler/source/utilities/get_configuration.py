import yaml

from .get_root import get_root


_configuration = {}


def get_configuration() -> dict:
    ''' Fetches the configuration dict, as loaded from configuration.yaml

    Returns
    -------
    configuration: dict
        the loaded configuration object
    '''

    global _configuration
    if not _configuration:

        # The point of using a configuration object is to not have to hard-code
        # paths and the like -- but the configuration object's path is the one
        # exception
        root = get_root()
        path = f'{root}/compiler/configuration/configuration.yaml'
        with open(path, 'r') as file:
            _configuration = yaml.safe_load(file.read())
    
    return _configuration