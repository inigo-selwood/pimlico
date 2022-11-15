from pimlico.grammar.terms import Reference

from ...utilities import test


def test_reference_parse_valid():

    text = "_azAZ09"
    constant = test.run_parser(Reference.parse, text)
    assert constant.text == "_azAZ09"