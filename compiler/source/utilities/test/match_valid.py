import text
import tools
import grammar


def match_valid(object, grammar: str) -> str:
    buffer = text.Buffer(grammar)

    success, match_text = object.match(buffer)
    assert success, f'grammar: \'{grammar}\''
    
    return match_text