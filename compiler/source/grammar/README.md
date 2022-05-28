# Grammar

A grammar guides the parsing process. 

- A complete grammar is called a schema 
- Each rule has one or more productions. Rules can have types, and be referenced by others.
- Productions are comprised of a sequence of terms and an optional expression.

```
program
├─ rule
│  ├─ production
│  │  ╰─ sequence [expression]
│  ╰─ ...
╰─ ...
```

## Term

See [terms](terms/README.md)

## Production

A production is a sequence of terms:

```
term0 ... termN
```

Productions can have an embedded C/C++ expression. When the sequence is matched, the expression is executed:

```
term0 ... termN {{
        // C/C++ expression
    }}
```

## Rule

A rule maps one or more productions to a named non-terminal. 

```
ruleName := term0 ... termN
```

If the production has an embedded expression, it's executed when that production matches:

```
ruleName := term0 ... termN {{
        // C/C++ expression
    }}
```

If the embedded expression returns a value to be used elsewhere, the rule needs to be typed:

```
ruleName <RuleType *> := term0 ... termN {{
        return (RuleType *)malloc(sizeof(RuleType));
    }}
```

Typed rules with no embedded expression will return default constructed values, or NULL for pointers.

Rules can match more than one production, where the first one that matches is always accepted:

```
ruleName := 
    - term0 ... termN
    - term0 ... termM
```

## Schema

A schema consists of comments, macros, and rules:

```
# Comment

# Macro
.include <strings.h>
.include "your_header.hpp"

# Rule
ruleName <RuleType *> := term0 ... termN {{
        // C/C++ expression
    }}

...
```
