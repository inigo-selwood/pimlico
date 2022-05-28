# Terms

### Choice

A choice of different terms, any of which will match

```
option0 | option1 | ... | optionN
```

### Constant

A string literal

```
'This is a constant'
```

The character `'` can be escaped as `\'`

### Intrinsic

Pimlico comes with some common tokens built in

Name             | Default Type   | Description
-----------------|----------------|---
`__character__`  | `char`         | Any ASCII value in the range `[ ~]`
`__identifier__` | `const char *` | A string of alphanumeric values or underscores, not starting in a digit
`__indent__`     | `bool`         | 4 spaces, or a horizontal tab
`__integer__`    | `uint32_t`     | A decimal integer
`__newline__`    | `bool`         | A newline, or carriage-return and newline
`__number__`     | `double`       | A number in IEEE754 scientific notation

### Range

Matches any ASCII value between the two given (inclusive)

```
# Lowercase letter
[az]

# Single digit
[09] 
```

The two characters `⎵` and `]` can be escaped as `' '` and `\]` respectively

### Reference

Refers to a rule defined elsewhere

```
# rule references otherRule
rule := otherRule
```

### Sequence

A series of terms expected to appear in order

```
('red' 'orange' 'yellow' 'green' 'blue' 'indigo' 'violet')
```

#### Ligatures

Under normal circumstantes, the parser ignores whitespace between each term in a sequence. Ligatures change this behaviour by telling the parser that the two terms are connected.

Grammar     | Matches
------------|---
`'a' 'b'`   | `ab` `a b` `a ... b`
`'a' . 'b'` | `ab`

### Set

A choice of single characters, any of which will match

```
`+-*/%`
```

The backtick character can be escaped with a backslash