import platform

from invoke import task


@task
def build(context):

    # Build
    command = [
        'clang',
        '-c',
        '-fPIC',
        'source/text/parse_buffer.c',
        '-o library.o'
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
        '-o library.so',
        'library.o'
    ]
    context.run(' '.join(command))


@task
def clean(context):
    context.run('rm -rf *.a *.o *.so')