import text
import tools


def parse_valid(grammar: str, parser: callable):
    buffer = text.Buffer(grammar)
    errors = tools.ErrorLog()

    result = parser(buffer, errors)
    
    messages = errors.__str__()
    assert result, messages
    assert buffer.finished(), buffer.position

    return result