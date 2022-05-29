import text
import tools
import grammar


def match_valid(object, grammar: str, pass_errors: bool = False):
    buffer = text.Buffer(grammar)

    # If an error log argument is required, we pass it and can give more 
    # informative assertion error messages
    match_text = None
    if pass_errors:
        errors = tools.ErrorLog
        success, match_text = object.match(buffer, errors)
        assert success, errors.__str__()
    
    # Otherwise, just run it and hope for the best
    else:
        success, match_text = object.match(buffer)
        assert success, f'grammar: \'{grammar}\' != \'{match_text}\''
    
    return match_text