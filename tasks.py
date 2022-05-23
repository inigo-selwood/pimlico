import os

import invoke


_root = None

def _run(context: invoke.context, command: list, path: str = '.'):
    global _root

    if _root is None:
        _root = os.path.dirname(os.path.abspath(__file__))

    with context.cd(f'{_root}/{path}'):
        context.run(' '.join(command))


@invoke.task
def test(context):

    command = [
        'python3',
        '-m',
        'pytest',
        '.'
    ]
    _run(context, command, 'source/')
    

@invoke.task
def coverage(context):

    command = [
        'mkdir',
        '-p',
        '../coverage/'
    ]
    _run(context, command, 'source/')

    command = [
        'echo',
        '"[run]\nomit = */test*"',
        '>',
        '../.coveragerc'
    ]
    _run(context, command, 'source/')

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

    command = [
        'rm',
        '-rf',
        '../.coveragerc',
        '.coverage'
    ]
    _run(context, command, 'source/')

        


@invoke.task
def clean(context):

    command = [
        'rm',
        '-rf',
        'coverage/',
        '.coveragerc',
        'source/.coverage',
    ]
    _run(context, command, 'source/')