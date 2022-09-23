from perivale import Buffer, ParseException
from grammar.terms import Sequence, Reference


def test_parse_sequence_valid():

    # Simple
    buffer = Buffer("a b")
    sequence = Sequence.parse(buffer, [])
    assert sequence
    assert buffer.finished()
    assert isinstance(sequence, Sequence)
    assert len(sequence.terms) == 2

    # Reduced sequence
    buffer = Buffer("a")
    reference = Sequence.parse(buffer, [])
    assert reference
    assert buffer.finished()
    assert isinstance(reference, Reference)

    # Enclosed sequence
    buffer = Buffer("(a b)")
    sequence = Sequence.parse(buffer, [])
    assert sequence
    assert buffer.finished()
    assert isinstance(sequence, Sequence)
    assert len(sequence.terms) == 2

    # Enclosed reduced sequence
    buffer = Buffer("(a)")
    reference = Sequence.parse(buffer, [])
    assert reference
    assert buffer.finished()
    assert isinstance(reference, Reference)


def test_parse_sequence_invalid():

    # Unexpected EOF or newline
    for test in ["(", "(\n"]:
        buffer = Buffer(test)
        errors = []
        assert Sequence.parse(buffer, errors) is None
        assert len(errors) == 1
        assert errors[0].message == "expected ')' to match this"
        assert errors[0].position.line == 1
        assert errors[0].position.column == 1

    # Empty sequence
    buffer = Buffer("()")
    errors = []
    assert Sequence.parse(buffer, errors) is None
    assert len(errors) == 1
    assert errors[0].message == "empty sequence"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1