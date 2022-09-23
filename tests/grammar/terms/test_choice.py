from perivale import Buffer, ParseException
from grammar.terms import Choice, Reference


def test_choice_parse_valid():

    # Single term (reduced)
    buffer = Buffer("a")
    term = Choice.parse(buffer, []) 
    assert term
    assert buffer.finished()
    assert isinstance(term, Reference)
    assert buffer.finished()

    # Simple choice
    buffer = Buffer("a | b | c")
    choice = Choice.parse(buffer, [])
    assert choice
    assert buffer.finished()
    assert isinstance(choice, Choice)
    assert len(choice.terms) == 3
    for term in choice.terms:
        assert isinstance(term, Reference)


def test_choice_parse_invalid():

    # Unexpected end-of-file 
    buffer = Buffer("a | ")
    errors = []
    assert Choice.parse(buffer, errors) is None

    message = "unexpected end-of-file following choice operator"
    assert errors[0].message == message
    assert errors[0].position.line == 1
    assert errors[0].position.column == 3

    # Unexpected newline 
    buffer = Buffer("a | \n")
    errors = []
    assert Choice.parse(buffer, errors) is None
    assert errors[0].message == "unexpected newline following choice operator"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 3

