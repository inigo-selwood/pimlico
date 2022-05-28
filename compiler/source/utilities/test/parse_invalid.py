import text
import tools


def parse_invalid(grammar: str, parser: callable):    
    buffer = text.Buffer(grammar)
    errors = tools.ErrorLog()

    result = parser(buffer, errors)
    assert not result, grammar

    return errors