from text import helpers


def test_valid():
    
    text = helpers.escape('\'\"\\\t\v\n\r')
    assert text == '\\\'\\\"\\\\\\t\\v\\n\\r'

    text = helpers.escape('`', custom_codes={'`': '\\`'})
    assert text == '\\`'