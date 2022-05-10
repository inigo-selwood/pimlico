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

### Range

Matches any ASCII value between the two given (inclusive)

```
[az] # Lowercase letter
[09] # Single digit
```

The two characters `⎵` and `]` can be escaped as `' '` and `\]` respectively

### Reference

Refers to a rule defined elsewhere

```
rule := otherRule # rule references otherRule
```

### Sequence

A series of terms expected to appear in order

```
('red' 'orange' 'yellow' 'green' 'blue' 'indigo' 'violet')
```

### Set

A choice of single characters, any of which will match

```
`+-*/`
```

The backtick character can be escaped with a backslash