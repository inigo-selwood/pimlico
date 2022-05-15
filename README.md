# Pimlico

A parser generator tool for C/C++

## Installation

#### Install the latest version of [Python](https://www.python.org/)

```
user:~$ sudo apt update
user:~$ sudo apt install python3.9
```

While this tool generates parsers for C/C++, the build tool itself uses Python

#### Install [invoke](https://www.pyinvoke.org/)

```
user:~$ pip3 install invoke
```

Invoke provides tidy Makefile-like functionality with clearer syntax, and is used to build the shared C library.

#### Install [clang](https://clang.llvm.org/) and [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

```
user:~$ sudo apt install clang clang-format
```

For Windows users, you'll want to install a similar C/C++ compiler like [MinGW](https://www.mingw-w64.org/) or [Cygwin](https://www.cygwin.com/).

If you're a Windoes user, or for whatever reason you don't want to use `clang`, the only thing you'll have to change is the compiler used in our ["Makefile"](library\tasks.py). The parser will still work without `clang-format`, but any inconsistent indentation in your grammar files will be carried through to the header file output. 

#### Clone or download the Pimlico repo

```
user:~$ git clone git@github.com:inigo-selwood/Pimlico.git && cd Pimlico
```

#### Build the shared C library

```
user:Pimlico$ (cd library && invoke build)
```

The Python compiler uses [`ctypes`](https://docs.python.org/3/library/ctypes.html) to call C functions from the shared library, and can't work without them. 

## Quickstart Guide

#### Create `grammar.peg`:

```
.include(iostream)

term := 'say hello' {
        std::cout << "hello world\n";
    }
```

Here you've created a single rule `term`. This term matches the string constant `"say hello"`, and binds it to an expression that prints `"hello world"` to stdout. When the constant is encountered, the expression is executed.

#### Build the grammar

```
user:Pimlico$ python3 source/main.py build grammar.peg --name hello_world
```

This creates the header file `hello_world.h` which you can include in your source.

#### Create `main.c`:

```
#include "hello_world.h"

int main() {
    pimlicoParse("say hello");
    return 0;
}
```

We're including the header file created in the last step, and invoking the parser we created

#### Build `main.c`:

```
user:Pimlico$ clang main.c -o hello_world
```

#### Run the executable:

```
user:Pimlico$ ./hello_world
hello world
```

Congratulations, you've just built you first Pimlico parser in fewer than a dozen lines.
