def indent_text(text: str, level: int, tab_width: int = 4) -> str:
    ''' Indents a block of text to a given level

    Arguments
    ---------
    text: str
        the text to indent
    level: int
        the indentation level to bring the text to
    tab_width: int = 4
        (optional) the number of spaces per tab
    
    Returns
    -------
    result: str
        the indented text
    '''

    lines = text.split('\n')
    indent = ' ' * (level * tab_width)
    return f'\n{indent}'.join(lines)
