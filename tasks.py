import os

from invoke import task


def _get_root():
    return os.path.dirname(os.path.abspath(__file__))


@task
def test(context):

    root = _get_root()

    with context.cd(f'{root}/source'):
        command = [
            'python3',
            '-m',
            'pytest',
            '.'
        ]

        root = _get_root()
        with context.cd(f'{root}/source/'):
            context.run(' '.join(command))
    

@task
def coverage(context):


    root = _get_root()
    with context.cd(root):

        command = [
            'mkdir',
            '-p',
            'coverage/'
        ]

        context.run(' '.join(command))

        command = [
            'echo',
            '"[run]\nomit = */test*"',
            '>',
            '.coveragerc'
        ]

        context.run(' '.join(command))

    with context.cd(f'{root}/source'):

        command = [
            'python3',
            '-m',
            'pytest',
            '--cov=.',
            '--cov-report=html:../coverage',
            '--cov-config=../.coveragerc',
            '.'
        ]

        context.run(' '.join(command))


@task
def clean(context):

    root = _get_root()

    with context.cd(root):

        command = [
            'rm',
            '-rf',
            'coverage/',
            '.coveragerc',
            'source/.coverage',
        ]

        context.run(' '.join(command))