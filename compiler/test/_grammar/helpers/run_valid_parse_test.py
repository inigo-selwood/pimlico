from text import ParseBuffer, ErrorBuffer


def run_valid_parse_test(grammar: str, parser: callable, arguments: dict = {}):
    ''' Runs a parse function, asserting success

    Arguments
    ---------
    grammar: str
        the text to parse
    parser: callable
        a parse function of type `ParseBuffer, ErrorBuffer -> Class`
    arguments: dict = {}
        an optional dictionary of arguments to forward to the parser
    '''
    
    buffer = ParseBuffer(grammar)
    errors = ErrorBuffer()

    result = parser(buffer, errors, **arguments)
    
    messages = errors.serialize(buffer)
    assert result, messages
    assert buffer.finished(), messages

    return result