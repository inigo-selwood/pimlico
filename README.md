# Pimlico

![Build](https://img.shields.io/endpoint?logo=github&style=plastic&url=https://gist.githubusercontent.com/inigo-selwood/2dda5da8e52e2be176c1df0f520d06c5/raw/pimlico-build-badge.json)
![Coverage](https://img.shields.io/endpoint?logo=github&style=plastic&url=https://gist.githubusercontent.com/inigo-selwood/2dda5da8e52e2be176c1df0f520d06c5/raw/pimlico-coverage-badge.json)

A simple lexer and parser toolchain

## Goals

+ Simple and human readable syntax
+ Thorough, flexible error reporting
+ Support for structured whitespace
+ Fast development iteration

### Sections

- [Grammar](#grammar)
    - [Terms](#terms)
    - [Instance hints](#instance-hints)
    - [Predicates](#predicates)

## Grammar

Pimlico uses specification files with the `.peg` format.

PEG combines a modified REGEX syntax with C++ embeddings. Files consist of a list of named rules, productions, and terms. Productions can bind terms to snippets of embedded C++ code, which are combined to produce a parser.

---

### Terms

The parser recognizes five kinds of terms: constants, ranges, references, sequences, and choices.

#### Constants

Constants are characters or strings. For example:

`semi_colon: ';'`

or

`return_keyword: 'return'`

#### Ranges

Ranges match ASCII values between two given characters (inclusive). A production rule for lowercase letters might use the range:

`lowercase: ['a' - 'z']`

#### References

References substitute for an already-defined rule

```
lowercase: ['a' - 'z']
uppercase: ['A' - 'Z']

letter: lowercase | uppercase
```

Here, the production rule for `letter` can be either `lowercase` or `uppercase`

#### Sequences and Choices

Subsequent terms form a `sequence`. Its terms must appear in the order given to match. A `choice` (terms separated by `|`) succeeds if any of its terms are present.

A choice can include any number of terms. Sequences and choices can be nested by enclosing them in parentheses. For example:

```
# Succeeds for '<' or '>', or '==', '!=', '<=', or '>='
comparison_operator: ('<' | '>') | ('=' | '!' | '<' | '>' '=')
```

---

### Instance hints

Terms can be followed by instance hints, defining how many instances to expect.

Hint    | Meaning
--------|---
`?`     | One, optional
`\*`    | Zero or more
`\+`    | One or more
`{n}`   | N instances
`{:n}`  | Up to N instances
`{n:}`  | N or more instances
`{m:n}` | Between M and N instances, inclusive

For example:

`number: '-'? [0 - 9]+ ('.' [0 - 9]+)? ('E' | 'e') '-'? [0 - 9]+`

**Note:** `term*` and `term?` will always succeed, because they are allowed to match zero times.

**Note:** `term*` and `term{0:}` are functionally equivalent, as are `term+` and `term{1:}`, and `term?` and `term{0 : 1}`

---

### Predicates

Predicated terms aren't consumed by the parser. They're evaluated as either true or false, but the parser won't increment its position in the text.

There are two types of predicate:

Name | Symbol | Function
-----|--------|---
And  | `&`    | Succeeds if the term matched
Not  | `!`    | Succeeds if the term **not** matched

For example:

```
vowel: 'a' | 'e' | 'i' | 'o' | 'u' | 'A' | 'E' | 'I' | 'O' | 'U'
consonant: !vowel ['a' - 'z'] | ['A' - 'Z']
```
