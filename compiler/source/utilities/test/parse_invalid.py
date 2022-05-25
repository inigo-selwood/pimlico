import text
import tools


def parse_invalid(grammar: str, parser: callable, arguments: dict = {}):    
    buffer = text.Buffer(grammar)
    errors = tools.ErrorLog()

    result = parser(buffer, errors, **arguments)
    assert not result, grammar

    return errors