import os

import invoke


_root = None


def _get_root():

    global _root
    if _root is None:
        _root = os.path.dirname(os.path.abspath(__file__))
    
    return _root


def _run(context: invoke.context, command: list, path: str = '.'):
    root = _get_root()

    # Move to the path specified, run the command
    with context.cd(f'{root}/{path}'):
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
    _run(context, command, 'source')
    

@invoke.task
def coverage(context):

    # Make sure coverage directory exists
    _run(context, ['mkdir', '-p', 'coverage/'])

    # Create .coveragerc to omit test files from output report
    command = [
        'echo',
        '"[run]\nomit = */test*"',
        '>',
        '.coveragerc'
    ]
    _run(context, command)

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
    _run(context, command, 'source')

    # Get rid of temp. coverage files
    command = [
        'rm',
        '-rf',
        '.coveragerc',
        'source/.coverage'
    ]
    _run(context, command)


@invoke.task
def clean(context):

    # Delete coverage generated files etc.
    command = [
        'rm',
        '-rf',
        'profiling/',
        'coverage/',
        '.coveragerc',
        'source/.coverage',
    ]
    _run(context, command)


@invoke.task
def profile(context):

    # Make sure coverage directory exists
    _run(context, ['mkdir', '-p', 'profiling/'])

    root = _get_root()
    grammar_path = f'{root}/../examples/pimlico.peg'

    # Profile run of application
    command = [
        'python3',
        '-m', 'cProfile',
        '-o', 'profiling/profile.dat',
        'source/main.py',
        'generate',
        grammar_path,
        '> /dev/null',
    ]
    _run(context, command)

    # Run the filter, generate an output graph
    command = [
        'gprof2dot',
        '-f', 'pstats',
        '-z', 'parse:7:parse',
        'profiling/profile.dat',
        '|',
        'dot',
        '-Tpng',
        '-o', 'profiling/profile.png',
    ]
    _run(context, command)