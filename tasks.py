import platform

from invoke import task


@task
def build(context):

    context.run('mkdir -p binaries')

    # Build
    command = [
        'clang',
        '-c',
        '-fPIC',
        'library/source/text/parse_buffer.c',
        '-o binaries/library.o'
    ]
    context.run(' '.join(command))

    # Assemble
    versioning_flag = 'soname'
    if platform.system() == 'Darwin':
        versioning_flag = 'install_name'
    
    command = [
        'clang',
        '-shared',
        f'-Wl,-{versioning_flag},library.so',
        '-o binaries/library.so',
        'binaries/library.o'
    ]
    context.run(' '.join(command))

@task
def coverage(context):

    context.run('mkdir -p coverage')

    with context.cd('compiler/test'):

        command = [
            'python3',
            '-m', 'pytest',
            '--cov=../source/',
            '--cov-report=html:../../coverage',
            '--cov-branch',
            '.'
        ]

        context.run(' '.join(command))

        context.run('rm .coverage')

@task
def clean(context):
    context.run('rm -rf binaries/ coverage/')