from grammar.terms import Reference

from ..helpers import run_valid_test


def test_simple():
    run_valid_test('azAZ09_', Reference.parse)