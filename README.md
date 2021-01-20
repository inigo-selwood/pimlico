# Pimlico

A simple parser for parsing expression grammars (PEGs).

Pimlico is written with the following goals in mind:
+ Simple and human readable syntax
+ Thorough and flexible error handling that allows the programmer to control formatting and reporting
+ A sensible balance between memory and computational overhead
+ Support for parsing whitespace structured text

The intent is to speed up the development of parsers and interpreters by reducing the amount of time the programmer has to spend on creating lexical and syntactical analysers, as well as catching and reporting errors as early as possible.

## Sections

- [Grammar](#grammar)
    - [Terms](#terms)
    - [Instance hints](#instance-hints)
    - [Predicates](#predicates)
    - [Inlining](#inlining)
    - [Name extension](#name-extension)
    - [Multi-line Statements](#multi-line-statements)
    - [Simple JSON Example](#simple-json-example)
- [Programming Interface](#programming-interface)
    - [Token Printer Example](#token-printer-example)

## Grammar

Pimlico uses grammar files written in a language that combines the features of EBNF with aspects of regular expression syntax. By convention, grammar files should have the extension `.peg`.

Grammar files consist of a list of production rules, each with a name, and a sequence of terms. The parser then tries to map rules to strings in the text being parsed, creating an abstract syntax tree (AST).

---

### Terms

The parser recognizes five kinds of terms: constants, ranges, references, sequences, and choices.

#### Constants

Constants are characters or strings that are expected to appear verbatim in the text being parsed. For example:

`semi_colon: ';'`

or

`return_keyword: 'return'`

#### Ranges

Ranges are shorthand for a range of acceptable ASCII values. A production rule for lowercase letters might use a range:

`lowercase: ['a' - 'z']`

#### References

The strength of PEGs is that one can define a rule, and then use it within other rules. Or perhaps, recursively nest them. Building on the previous example:

```
lowercase: ['a' - 'z']
uppercase: ['A' - 'Z']

letter: lowercase | uppercase
```

Here, the production rule for `letter` is equivalent to a character as defined in either `lowercase` or `uppercase`.

#### Sequences and Choices

A series of terms separated by whitespace is called a `sequence`. For a sequence to succeed, each of its terms must appear in order. While a sequence requires all of its terms to be present, a `choice` only requires one. Borrowing again from regular expression syntax, a vertical pipe character between two terms denotes a choice.

A choice can include any number of terms. Sequences and choices can be nested by enclosing them in parentheses. For example:

```
# Succeeds for '<' or '>', or '==', '!=', '<=', or '>='
comparison_operator: ('<' | '>') | ('=' | '!' | '<' | '>' '=')
```

---

### Instance hints

Terms can be followed by instance hints, which tell the parser how many instances to expect.

Hint    | Meaning
--------|---
?       | One, optional
\*      | Zero or more
\+      | One or more

So for example, a production rule for scientific notation might look like this:

`number: '-'? [0 - 9]+ ('.' [0 - 9]+)? ('E' | 'e') '-'? [0 - 9]+`

Note that `term*` and `term?` will always succeed, because they are allowed to match zero times.

Other numbers of repetitions can be indicated using curly brackets:

```
# Exactly n repetitions
term{n}

# Between m and n repetitions, inclusive
term{m : n}

# At most n repetitions
term{:n}

# At least m repetitions
term{m:}
```

Note that `term*` and `term{0:}` are functionally equivalent, as are `term+` and `term{1:}`, and `term?` and `term{0 : 1}`

---

### Predicates

Predicates aren't necessary for a language with a comprehensive specification, but they can be helpful in two regards:

- Speeding up parsing by implementing lookahead checks
- Clarifying syntax with the "everything-but" idiom

Predicated terms aren't consumed by the parser. That's to say, they're evaluated as either true or false, but the parser won't increment its position in the text.

There are two types of predicate:

Name    | Symbol    | Function
--------|-----------|---
And     | `&`       | Succeeds if the predicated term is matched
Not     | `!`       | Succeeds if the predicated term is **not** matched

Terms prefixed with a predicate symbol will be evaluated as such.

The not operator is used in the "everything-but" idiom, primarily in a sequence with a range. For example:

```
vowel: 'a' | 'e' | 'i' | 'o' | 'u' | 'A' | 'E' | 'I' | 'O' | 'U'
consonant: !vowel ['a' - 'z'] | ['A' - 'Z']
```

In this example, `consonant` will match any lower or uppercase letter which isn't a vowel. This could be implemented with a series of ranges that side-step vowels, but this method is neater and conveys intent better.

The more complicated use of predicates is, as stated above, the speeding up of parsing. To understand this, it helps to understand how the parser matches rules to text. It begins with a list of all rules, and reduces the list of candidates as it consumes characters until a single solution is found (or isn't, in which case it will report an error). This is fast for rules which are easily distinguished, but for longer and more complex rules, evaluating multiple candidates in parallel is inevitably slower.

By using predicates, the programmer can make their intent clearer in their grammar specification, and potentially save the parser time.

Predicates can also be necessary if the parser is returning 'multiple candidate' errors, wherein more than one rule matches the string being parsed, with no way to distinguish between them. However, this is often due to ambiguity in the specification itself, and the programmer may wish to re-evaluate their approach.

---

### Inlining

To promote the use of rules for clarity, they can be inlined.

Consider the following extension of a previous example:

```
digit: ['0' - '9']
number: '-'? digit+ ('.' digit+)? ('E' | 'e') '-'? digit+
```

For the string `3.141E0`, the parser would generate the following output:

`number(digit(3), '.', digit('1'), digit('4'), 'E', digit('0'))`

This might be the desired behaviour -- but equally we might not want a distinct digit term instance for each digit character parsed. Rules whose names begin with an underscore are 'inlined', and will not appear in the parser's output.

```
_digit: ['0' - '9']
number: '-'? _digit+ ('.' _digit+)? ('E' | 'e') '-'? _digit+
```

Now the parsed result would look like this:

`number(3.14E0)`

---

### Name Extension

Name extension is a tool for clarity in your grammar specification. Grammars can be specified equivalently without; however, it helps keep line lengths short, and rule names readable and intuitive.

```
operator...
    arithmetic...
        binary: '+' | '-' | '*' | '/' | '%'
        bitwise: '&' | '|' | '^'
    assignment: arithmetic_operator? '='
```

Notice how in this example, we've used the reference `arithmetic_operator`, which is equivalent to `binary_arithmetic_operator | bitwise_arithmetic_operator`. Similarly, you could use the reference `operator` to mean any of the rules nested underneath it.

The scope of extended names is determined by indentation.

---

### Multi-line Statements

Sometimes a rule might exceed the programmer's arbitrary line length preference, in which case a two-tab increase in indentation can be used to split the rule over multiple lines. In this example, the strings are defined as enclosed in either single or double quotes -- where in a single-quote-enclosed string, double quotes needn't be escaped, and vice versa. An implementation like this might be divided for clarity.

```
string: ('\'' ((!'\' [' ' - 'z']) | escape_code | '\\\'')* '\'') |
        ('\"' ((!'\' [' ' - 'z']) | escape_code | '\\\'')* '\")
escape_code: ('\\' | 'b' | 'n' | 'r' | 't') '\''
```

---

### Simple JSON Example

```
pair: '\"' [' ' - '~'] '\"' ':' term
term...
    object: '{' pair (',' pair)* '}'
    array: '[' term (',' term)* ']'
    string: '\"' [' ' - '~'] '\"'
    number: ['0' - '9']
    boolean: 'true' | 'false'
    null: 'null'
```

## Programming interface

The programming interface is intended to be C++ idiomatic. The main classes in the Pimlico namespace which the programmer will interact with are:

Class           | Purpose
----------------|---
`Specification` | Parsing and holding a grammar specification
`SyntaxError`   | Reporting syntax errors either in the specification or text parsed using it
`SyntaxTree`    | Holding tokens that've been parsed per the specification
`Token`         | Instances of grammar rules, populated with either text or other tokens

### Token Printer Example

``` c++
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "pimlico.hpp"
using namespace Pimlico;

// Report syntax errors
void print_error(const std::string &file_name, const SyntaxError &error) {
    std::cerr << file_name << " [" << error.line << ":" << error.column << "] ";
    std::cerr << error.message << "\n";
    std::cerr << error.text << "\n";

    for(unsigned int index = 0; index < error.column; index += 1)
        std::cerr << " ";
    std::cerr << "^";
}

// Basic token printer
void print_token(const Token &token) {
    std::cout << token.name << "(";

    // Print terminal tokens
    if(token.terminal)
        std::cout << terminal.value();

    // Recursively print non-terminal tokens' sub-tokens
    else {
        std::vector<Token> &tokens = token.values();
        const unsigned int token_count = tokens.size();
        for(unsigned int index = 0;  index < token_count; index += 1) {
            print_token(tokens[index]);
            if(index + 1 < token_count)
                std::cout << ", ";
        }
    }

    std::cout << ")"
}

int main(int argument_count, char *argument_values[]) {

    // Check the program has the right number of arguments
    if(argument_count != 3) {
        std::cerr << "wrong number of arguments\n";
        return -1;
    }

    // Open the grammar stream
    std::string grammar_filename = argument_values[1];
    std::ifstream grammar_stream(grammar_filename);
    if(grammar_stream.is_open() == false) {
        std::cerr << "couldn't open grammar file\n";
        return -2;
    }

    // Create a specification
    std::string grammar_string((std::istreambuf_iterator<char>(grammar_stream)),
                 std::istreambuf_iterator<char>());

    std::vector<SyntaxError> syntax_errors;
    const Specification specification =
            Specification::parse(grammar_string, syntax_errors);
    if(specification == nullptr) {
        std::cerr << "error parsing grammar specification\n";
        for(const SyntaxError &syntax_error : syntax_errors)
            print_error(grammar_filename, syntax_error);
        return -3;
    }

    // Open the source stream
    std::string source_filename = argument_values[2];
    std::ifstream source_stream(source_filename);
    if(source_stream.is_open() == false) {
        std::cerr << "couldn't open source file\n";
        return -4;
    }

    // Create a syntax tree
    std::string grammar_string((std::istreambuf_iterator<char>(source_stream)),
                 std::istreambuf_iterator<char>());

    const SyntaxTree tree =
            SyntaxTree::parse(specification, source_string, syntax_errors);
    if(tree.valid == false) {
        std::cerr << "error parsing source\n";
        for(const auto &syntax_error : syntax_errors)
            print_error(source_filename, syntax_error);
        return -5;
    }

    // Recursively print the tree's tokens
    print_token(tree.root);

    return 0;
}
```

This boilerplate snippet shows how a text file can be split into tokens using a custom grammar, as well as how the programmer can leverage the flexible error handling system to report syntax errors in their own custom format.

The function `print_token` shows how terminal and non-terminal tokens can be differentiated between, and cast as such. It also shows how the interface exposes the name of the rule corresponding to the token, and its value(s).
