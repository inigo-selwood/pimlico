from perivale import Buffer

from pimlico.grammar.terms import Reference

from ...utilities import test


def test_reference_parse_valid():
    text = "_azAZ09"
    reference = test.run_parser(Reference.parse, text)
    assert reference.text == "_azAZ09"


def test_reference_serialize():
    text = "_azAZ09"
    term = Reference.parse(Buffer(text), [])
    assert f"{term}" == text