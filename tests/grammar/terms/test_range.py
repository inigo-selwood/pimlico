from perivale import Buffer, ParseException
from grammar.terms import Range


def test_range_parse_valid():

    # Simple
    buffer = Buffer("[az]")
    range = Range.parse(buffer, [])
    assert range
    assert buffer.finished()
    assert isinstance(range, Range)
    assert range.bounds == ("a", "z")
    
    # With space
    buffer = Buffer("[' '~]")
    range = Range.parse(buffer, [])
    assert range
    assert buffer.finished()
    assert range.bounds == (" ", "~")
    
    # With whitepace
    buffer = Buffer("[ a z ]")
    range = Range.parse(buffer, [])
    assert range
    assert buffer.finished()
    assert range.bounds == ("a", "z")
    
    # With escaped bracket
    buffer = Buffer(r"[[\]]")
    range = Range.parse(buffer, [])
    assert range
    assert buffer.finished()
    assert range.bounds == ("[", "]")


def test_range_parse_invalid():

    # One missing value
    buffer = Buffer("[a]")
    errors = []
    assert not Range.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected range value"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 3

    # Two missing values
    buffer = Buffer("[]")
    errors = []
    assert not Range.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected range value"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 2

    # Missing closing bracket
    buffer = Buffer("[abc]")
    errors = []
    assert not Range.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected ']'"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 4

    # Illogical bounds values
    for test in ["[aa]", "[za]"]:
        buffer = Buffer(test)
        errors = []
        assert not Range.parse(buffer, errors)
        assert len(errors) == 1
    assert errors[0].message == "illogical range bounds"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1