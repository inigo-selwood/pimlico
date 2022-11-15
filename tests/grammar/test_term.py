from perivale import Buffer

from pimlico.grammar import Term
from pimlico.grammar.terms import (Choice, 
        Constant, 
        Range, 
        Reference, 
        Sequence, 
        Set)

from ..utilities import test


def test_term_parse_valid():

    # Basic terms
    typed_terms = {
        "(a | b)": Choice,
        "'constant'": Constant,
        "[az]": Range,
        "reference": Reference,
        "(a b)": Sequence,
        "`abc`": Set,
    }

    predicates = {
        "!": "not",
        "&": "and",
    }

    hints = {
        "?": (0, 1),
        "*": (0, -1),
        "+": (1, -1),
        "<2>": (2, 2),
        "<2:>": (2, -1),
        "<:2>": (0, 2),
        "<2:3>": (2, 3),
    }

    for term, term_type in typed_terms.items():
        for suffix, hint in hints.items():

            for prefix, predicate in predicates.items():
                text = f"{prefix}{term}{suffix}"
                parsed_term = test.run_parser(Term.parse, text)

                assert isinstance(parsed_term, term_type)
                assert parsed_term.predicate == predicate
                assert parsed_term.hint == hint
            
            text = f"binding:{term}{suffix}"
            parsed_term = test.run_parser(Term.parse, text)

            assert isinstance(parsed_term, term_type)
            assert parsed_term.binding == "binding"
            assert parsed_term.hint == hint


def test_term_parse_hint_invalid():

    # Incomplete hint
    for text in ["<", "<1", "<:1", "<1:", "<1:2"]:
        errors = test.run_parser_fail(Term.parse_hint, text)
        excerpts = [
            ("expected '>'", (-1, -1)),
            ("to match this", (1, 1)),
        ]
        assert test.error_present(errors, excerpts)

    # Redundant hint
    excerpts = [("redundant hint", (1, 1))]
    errors = test.run_parser_fail(Term.parse_hint, "<1>")
    assert test.error_present(errors, excerpts)

    # Illogical hints
    for text in ["<0>", "<:0>", "<2:1>"]:
        excerpts = [("illogical hint", (1, 1))]
        errors = test.run_parser_fail(Term.parse_hint, text)
        assert test.error_present(errors, excerpts)
    
    # Invalid hint
    excerpts = [("invalid hint", (1, 1))]
    errors = test.run_parser_fail(Term.parse_hint, "<>")
    assert test.error_present(errors, excerpts)