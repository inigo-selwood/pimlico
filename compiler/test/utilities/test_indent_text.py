from pytest import raises

from utilities import indent_text


def test_indent():
    assert indent_text('.\n.', 1) == '.\n    .'
    assert indent_text('.\n.', 2) == '.\n        .'

def test_indent_custom_tab():
    assert indent_text('.\n.', 1, tab_width=2) == '.\n  .'
