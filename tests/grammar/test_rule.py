from perivale import Buffer

from pimlico.grammar import Rule
from pimlico.grammar.terms import Reference

from ..utilities import test


def test_rule_parse_valid():

    # Single production
    text = "rule\n\t:= term"
    rule = test.run_parser(Rule.parse, text)
    assert rule.name == "rule"
    assert len(rule.productions) == 1
    term, _ = rule.productions[0]
    assert isinstance(term, Reference)

    # Multiple productions
    text = """rule
    := term0
    := term1"""
    rule = test.run_parser(Rule.parse, text)
    assert rule.name == "rule"
    assert len(rule.productions) == 2
    for index in range(2):
        term, _ = rule.productions[index]
        assert isinstance(term, Reference)
    
    # With expression
    text = """rule
    := term {
        return "hello world"
    }"""
    rule = test.run_parser(Rule.parse, text)
    assert rule.name == "rule"
    assert len(rule.productions) == 1
    term, expression = rule.productions[0]
    assert isinstance(term, Reference)
    assert expression == "return \"hello world\""


def test_rule_parse_invalid():

    # Invalid indentation
    text = """rule
:= term"""
    errors = test.run_parser_fail(Rule.parse, text)
    excerpts = [("invalid indentation", (2, 1))]
    assert test.error_present(errors, excerpts)

    # Missing term
    text = """rule
    :="""
    errors = test.run_parser_fail(Rule.parse, text)
    excerpts = [("expected a term", (-1, -1))]
    assert test.error_present(errors, excerpts)

    # Empty expression
    text = """rule
    := term {}"""
    errors = test.run_parser_fail(Rule.parse, text)
    excerpts = [("empty expression", (2, 13))]
    assert test.error_present(errors, excerpts)

    # No productions
    text = "rule"
    errors = test.run_parser_fail(Rule.parse, text)
    excerpts = [("a rule expects productions", (1, 1))]
    assert test.error_present(errors, excerpts)
