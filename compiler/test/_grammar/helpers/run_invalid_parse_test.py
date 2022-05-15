from text import ParseBuffer, ErrorBuffer


def run_invalid_parse_test(grammar: str, 
        parser: callable, 
        instances: list, 
        arguments: dict = {}):
    
    ''' Runs a parse function with a given grammar, asserts failure

    Arguments
    ---------
    grammar: str
        the text to try and parse
    parser: callable
        a function of the form `ParseBuffer, ErrorBuffer -> Class`
    instances: list
        the errors expected, each one a tuple of message and position, of the
        form `str * (int * int)`
    must_finish: bool = False
        whether to check that the buffer has been fully consumed
    arguments: dict = {}
        an optional dictionary of arguments to forward to the parser
    '''
    
    buffer = ParseBuffer(grammar, '#')
    errors = ErrorBuffer()

    result = parser(buffer, errors, **arguments)
    assert not result, grammar

    instance_count = len(instances)
    assert len(errors.instances) == instance_count
    for index in range(instance_count):
        _, true_message, excerpt = errors.instances[index]
        _, true_position = excerpt
        test_message, test_position = instances[index]

        assert true_message == test_message
        assert (true_position.line, true_position.column) == test_position