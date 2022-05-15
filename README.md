# Pimlico

A parser generator tool for C/C++

## Installation

Install the latest version of [Python](https://www.python.org/)

```
user:~$ sudo apt update
user:~$ sudo apt install python3.9
```

Install [invoke](https://www.pyinvoke.org/)

```
user:~$ pip3 install invoke
```

Install [clang](https://clang.llvm.org/) and [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

```
user:~$ sudo apt install clang clang-format
```

Clone or download the Pimlico repo

```
user:~$ git clone git@github.com:inigo-selwood/Pimlico.git && cd Pimlico
```

Build the shared C library

```
user:Pimlico$ (cd library && invoke build)
```

## Quickstart Guide

First, create `grammar.peg`:

```
.include(iostream)

term := 'say hello' {
        std::cout << "hello world\n";
    }
```

Here you've created a single rule `term`. This term matches the string constant `"say hello"`, and binds it to an expression that prints `"hello world"` to stdout. When the constant is encountered, the expression is executed.

---

Now build the grammar

```
user:Pimlico$ python3 source/main.py build grammar.peg --name hello_world
```

This creates the header file `hello_world.h` which you can include in your source.

---

Create `main.c`:

```
#include "hello_world.h"

int main() {
    pimlicoParse("say hello");
    return 0;
}
```

We're including the header file created in the last step, and invoking the parser we created

---

Build `main.c`:

```
user:Pimlico$ clang main.c -o hello_world
```

---

Run the executable:

```
user:Pimlico$ ./hello_world
hello world
```
