import yaml

from .get_root import get_root


_configuration = {}


def get_configuration():

    global _configuration
    if not _configuration:

        root = get_root()
        path = f'{root}/compiler/configuration/configuration.yaml'
        with open(path, 'r') as file:
            _configuration = yaml.safe_load(file.read())
    
    return _configuration