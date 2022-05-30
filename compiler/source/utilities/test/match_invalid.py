import text
import tools
import grammar


def match_invalid(term: grammar.Term, grammar: str):
    buffer = text.Buffer(grammar)
    
    term_match, _ = term.match(buffer)
    assert not term_match, f'grammar: \'{grammar}\''