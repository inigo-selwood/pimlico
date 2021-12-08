# Pimlico

A simple parser for parsing expression grammars (PEGs).

## Overview

Pimlico is written with the following goals in mind:
+ Simple and human readable syntax
+ Thorough and flexible error handling that allows the programmer to control formatting and reporting
+ A sensible balance between memory and computational overhead
+ Support for parsing whitespace structured text

The intent is to speed up the development of parsers and interpreters by reducing the amount of time the programmer has to spend on creating lexical and syntactical analysers, as well as catching and reporting errors as early as possible.

### Sections

- [Grammar](#grammar)
    - [Terms](#terms)
    - [Instance hints](#instance-hints)
    - [Predicates](#predicates)

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
`?`     | One, optional
`\*`    | Zero or more
`\+`    | One or more
`{n}`   | N instances
`{:n}`  | Up to N instances
`{n:}`  | N or more instances
`{m:n}` | Between M and N instances, inclusive

So for example, a production rule for scientific notation might look like this:

`number: '-'? [0 - 9]+ ('.' [0 - 9]+)? ('E' | 'e') '-'? [0 - 9]+`

**Note:** `term*` and `term?` will always succeed, because they are allowed to match zero times.

**Note:** `term*` and `term{0:}` are functionally equivalent, as are `term+` and `term{1:}`, and `term?` and `term{0 : 1}`

---

### Predicates

Predicates aren't necessary for a language with a comprehensive specification, but they can be helpful in two regards:

- Speeding up parsing by implementing lookahead checks
- Clarifying syntax with the "everything-but" idiom

Predicated terms aren't consumed by the parser. They're evaluated as either true or false, but the parser won't increment its position in the text.

There are two types of predicate:

Name | Symbol | Function
-----|--------|---
And  | `&`    | Succeeds if the predicated term is matched
Not  | `!`    | Succeeds if the predicated term is **not** matched

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
