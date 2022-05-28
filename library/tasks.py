import os
import platform
from glob import glob

from pathlib import Path

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
    with context.cd(f'{_root}/{path}'):
        context.run(' '.join(command))


@invoke.task
def build(context):

    # Create libraries dir
    _run(context, ['mkdir', '-p', 'libraries'])

    # Glob source files
    root = _get_root()
    source_path = f'{root}/source/**/*.c'
    source_files = glob(source_path)
    if not source_files:
        raise Exception(f'no source files match \'{source_path}\'')
    
    print('compiling sources:')
    
    # Compile an object file from each source file
    object_files = {}
    for source_file in source_files:
        file_name = os.path.splitext(os.path.basename(source_file))[0]
        output_path = f'{root}/libraries/{file_name}.o'
        object_files[file_name] = output_path

        print(f'- {source_file}')

        # Build
        command = [
            'clang',
            '-c',
            '-fPIC',
            f'-o', output_path,
            f'-I{root}/source',
            source_file,
        ]
        _run(context, command)
    
    # Link object files into one big one
    print('linking')
    command = [
        'ld',
        '-r', 
        '-o', f'{root}/libraries/library.o',
    ]
    command.extend(object_files.values())
    _run(context, command)

    # Get the right flag name, depending on the target platform
    versioning_flag = 'soname'
    if platform.system() == 'Darwin':
        versioning_flag = 'install_name'
    
    # Make library shared
    print('making shared')
    command = [
        'clang',
        '-shared',
        f'-Wl,-{versioning_flag},library.so',
        '-o', f'{root}/libraries/library.so',
        f'{root}/libraries/library.o'
    ]
    _run(context, command)


@invoke.task
def clean(context):
    command = [
        'rm',
        '-rf',
        'libraries',
        'binaries',
        'temporary',
        'test',
    ]
    _run(context, command)

@invoke.task
def test(context):

    root = _get_root()

    if not os.path.exists(f'{root}/test/catch.hpp'):

        # Make directories, one for the git repo we're cloning, and one to put
        # the source files in once downloaded
        _run(context, ['mkdir', '-p', f'{root}/test'])
        _run(context, ['mkdir', '-p', f'{root}/libraries'])

        # Just a reminder to the developer not to stage the unity test suite
        command = ['echo', '*', '>', f'{root}/test/.gitignore']
        _run(context, command)

        # Clone the Unity git repo
        catch_repo = 'https://raw.githubusercontent.com/catchorg/Catch2/v2.x'
        print('downloading catch2.0')
        command = [
            'curl',
            f'{catch_repo}/single_include/catch2/catch.hpp',
            '>',
            f'{root}/test/catch.hpp'
        ]
        _run(context, command)

    command = [
        'clang++', 
        '-std=c++17',
        '-x', 'c++-header',
        f'{root}/test/catch.hpp',
        '-emit-pch',
        '-o', f'{root}/test/catch.hpp.pch',
    ]
    _run(context, command)

    # Glob source files
    root = _get_root()
    test_pattern = f'{root}/source/**/test_*.cpp'
    test_files = glob(test_pattern)
    if not test_files:
        raise Exception(f'no test source files match \'{test_pattern}\'')

    # Create a folder for storing the compiled test binaries
    _run(context, ['mkdir', '-p', f'{root}/binaries'])
    
    binaries = []
    for test_file in test_files:
        file_name = os.path.splitext(os.path.basename(test_file))[0]
        output_path = f'{root}/binaries/{file_name}'
        binaries.append(output_path)

        # clang++ -include-pch header.pch source.cpp -o source -x c++
        command = [
            'clang++',
            '-std=c++17',
            '-include-pch', f'{root}/test/catch.hpp.pch',
            '-x', 'c++',
            f'-I{root}/source',
            f'-I{root}/test',
            '-o', output_path,
            test_file,
        ]
        _run(context, command)
    
    for binary in binaries:
        _run(context, [binary])