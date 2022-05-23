import os

import invoke


_root = None


def _run(context: invoke.context, command: list, path: str = '.'):
    global _root

    # Get root, relative to this file
    if _root is None:
        _root = os.path.dirname(os.path.abspath(__file__))

    # Move to the path specified, run the command
    with context.cd(f'{_root}/{path}'):
        context.run(' '.join(command))


@invoke.task
def test(context):

    # Run pytest
    command = [
        'python3',
        '-m',
        'pytest',
        '.'
    ]
    _run(context, command, 'source/')
    

@invoke.task
def coverage(context):

    # Make sure coverage directory exists
    command = [
        'mkdir',
        '-p',
        '../coverage/'
    ]
    _run(context, command, 'source/')

    # Create .coveragerc to omit test files from output report
    command = [
        'echo',
        '"[run]\nomit = */test*"',
        '>',
        '../.coveragerc'
    ]
    _run(context, command, 'source/')

    # Run the coverage analysis
    command = [
        'python3',
        '-m',
        'pytest',
        '--cov=.',
        '--cov-report=html:../coverage',
        '--cov-config=../.coveragerc',
        '.'
    ]
    _run(context, command, 'source/')

    # Get rid of temp. coverage files
    command = [
        'rm',
        '-rf',
        '../.coveragerc',
        '.coverage'
    ]
    _run(context, command, 'source/')


@invoke.task
def clean(context):

    # Delete coverage generated files etc.
    command = [
        'rm',
        '-rf',
        'coverage/',
        '.coveragerc',
        'source/.coverage',
    ]
    _run(context, command, 'source/')