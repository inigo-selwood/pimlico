from perivale import Buffer, ParseException
from grammar.terms import Reference


def test_reference_parse_valid():

    # Simple
    buffer = Buffer("azAZ09_")
    reference = Reference.parse(buffer, [])
    assert reference
    assert buffer.finished()
    assert isinstance(reference, Reference)
    assert reference.text == "azAZ09_"