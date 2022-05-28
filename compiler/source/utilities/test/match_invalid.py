import text
import tools
import grammar


def match_invalid(term: grammar.Term, grammar: str, pass_errors: bool = False):
    buffer = text.Buffer(grammar)

    # If an error log argument is required, we pass it and can give more 
    # informative assertion error messages
    result = None
    if pass_errors:
        errors = tools.ErrorLog
        term_match, _ = term.match(buffer, errors)
        
        assert not term_match
        result = errors
    
    # Otherwise, just run it and hope for the best
    else:
        term_match, _ = term.match(buffer)
        assert not term_match, grammar
    
    return result