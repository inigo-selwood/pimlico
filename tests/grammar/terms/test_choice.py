from pimlico.grammar.terms import Choice, Reference

from ...utilities import test


def test_choice_parse_valid():

    # Simple
    choice = test.run_parser(Choice.parse, "a | b")
    assert isinstance(choice, Choice)

    # Reduced
    reference = test.run_parser(Choice.parse, "a")
    assert isinstance(reference, Reference)


def test_choice_parse_invalid():

    # Missing value after pipe
    errors = test.run_parser_fail(Choice.parse, "a |")
    assert test.error_present(errors, [("expected a term", (-1, -1))])