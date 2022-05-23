import text
import tools


def parse_valid(grammar: str, parser: callable, arguments: dict = {}):
    buffer = text.Buffer(grammar)
    errors = tools.ErrorLog()

    result = parser(buffer, errors, **arguments)
    
    messages = errors.__str__()
    assert result, messages
    assert buffer.finished(), messages

    return result