from perivale import Buffer, ParseException
from grammar.terms import Set


def test_parse_set_valid():

    buffer = Buffer("`+-*/%`")
    set = Set.parse(buffer, [])
    assert set
    assert buffer.finished()
    assert isinstance(set, Set)
    assert set.values == "+-*/%"


def test_parse_set_invalid():

    # Empty set
    buffer = Buffer("``")
    errors = []
    assert not Set.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "empty set"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1

    # Duplicate in set
    buffer = Buffer("`aa`")
    errors = []
    assert not Set.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "duplicate character(s) in set: 'a'"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1

