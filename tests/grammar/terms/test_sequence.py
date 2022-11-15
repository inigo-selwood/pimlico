from perivale import Buffer

from pimlico.grammar.terms import Sequence, Reference

from ...utilities import test


def test_sequence_parse_valid():

    # Simple
    choice = test.run_parser(Sequence.parse, "a b")
    assert isinstance(choice, Sequence)

    # Reduced
    reference = test.run_parser(Sequence.parse, "a")
    assert isinstance(reference, Reference)


def test_sequence_parse_invalid():

    # Missing closing bracket
    errors = test.run_parser_fail(Sequence.parse, "(a")
    excerpts = [
        ("expected ')'", (-1, -1)),
        ("to match this", (1, 1)),
    ]
    assert test.error_present(errors, excerpts)


def test_sequence_serialize():
    sequences = [
        "a b",
        "a (b | c)",
    ]

    for text in sequences:
        term = Sequence.parse(Buffer(text), [])
        assert f"{term}" == text