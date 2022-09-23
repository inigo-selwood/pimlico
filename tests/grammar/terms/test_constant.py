from perivale import Buffer, ParseException
from grammar.terms import Constant


def test_constant_parse_valid():

    buffer = Buffer("'constant'")
    constant = Constant.parse(buffer, [])
    assert constant
    assert buffer.finished()
    assert isinstance(constant, Constant)
    assert constant.text == "constant"


def test_constant_parse_invalid():

    # Unexpected EOF
    buffer = Buffer("'")
    errors = []
    assert Constant.parse(buffer, errors) is None
    assert len(errors) == 1
    assert errors[0].message == "unexpected end-of-file"
    assert errors[0].position.line == 1
    assert errors[0].position.column == -1

    # Unexpected newline
    buffer = Buffer("'\n'")
    errors = []
    assert Constant.parse(buffer, errors) is None
    assert len(errors) == 1
    assert errors[0].message == "unexpected newline"
    assert errors[0].position.line == 1
    assert errors[0].position.column == -1

    # Empty constant
    buffer = Buffer("''")
    errors = []
    assert Constant.parse(buffer, errors) is None
    assert len(errors) == 1
    assert errors[0].message == "empty constant"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1