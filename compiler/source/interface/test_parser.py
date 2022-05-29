import interface


def test_add_command():

    parser = interface.Parser('test')
    command = interface.Command('test', 'for testing', None)
    parser.add_command(command)

    assert 'test' in parser.commands

def test_usage():
    
    parser = interface.Parser('')

    # With one command
    command = interface.Command('test', 'for testing', None)
    parser.add_command(command)
    usage = parser.usage()
    assert usage == 'usage: pimlico [--help] {test} ...'

    # With two commands
    command = interface.Command('other-test', 'for more testing', None)
    parser.add_command(command)
    usage = parser.usage()
    assert usage == 'usage: pimlico [--help] {test, other-test} ...'


def test_help():
    parser = interface.Parser('for testing')

    command = interface.Command('test', 'for testing', None)
    parser.add_command(command)
    
    help = parser.help()
    print(help)
    assert help == ('usage: pimlico [--help] {test} ...'
            '\n'
            '\nfor testing'
            '\n'
            '\ncommands:'
            '\n  test    for testing')


def test_run():
    pass