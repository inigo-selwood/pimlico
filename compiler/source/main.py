import interface
from text import ErrorBuffer


if __name__ == '__main__':
    errors = ErrorBuffer()

    success = interface.parse(errors)
    if not success:
        errors.serialize()