# Grammar

A grammar guides the lexing process. 

- A complete grammar is called a program, and is comprised of rules. 
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

A production is a sequence of terms, followed by an optional expression. If the terms match, the expression is executed.

```
([az] | [AZ] | [09] | `-_.`)+ '@outlook.com' {{
    std::cout << "you entered an alphanumeric email address"; 
}}
```

**Note:** In this example we've used `std::cout`. In the context of a program, this dependency has to be imported, which will be explained later.

## Rule

Rules group one or more productions together like a list of options. They're the building blocks of programs -- reusable units of grammar which can reference each other.
For parsing to succeed, the whole text has to reduce to a single rule.

When there's only one production, it's written inline:

```
alphanumericEmail := ([az] | [AZ] | [09] | `-_.`)+ '@outlook.com'
```

---

If the rule can have more than one form, they're enumerated like a list:

```
alphanumericEmailOrWebsite :=
    - ([az] | [AZ] | [09] | `-_.`)+ '@outlook.com'
    - 'https://www.' ([az] | [AZ] | [09] | `-_.`)+ '.com
```

In the above case, both `name@outlook.com` and `https://www.name.com` will match.

---

To return custom values from production expressions, for use in other rules, the rule must be typed:

```
.include(string)
.include(assignment.hpp)

integer <int> := [09]+ {{
        return std::stoi({% __text__ %});
    }}

integerAssignment <Assignment> := '=' (value: integer) {{
        return Assignment({% value %});
    }}
```

In this example we're using `std::stoi` and a custom type `Assignment`. We have to import their declarations with the `.include` macro

## Program

Finally, we can bring multiple rules together to form a complete program:

```
# These macros include the stdlib string header, and a custom header 
# declaration.hpp
.include(string)
.include(declaration.hpp)

# Lowercase and number are rules with one production. Each production has one 
# term, and no expression
lowercase := [az]
number := [09]

# Letter is a rule with two productions. One production references the rule 
# lowercase, and the other defines a range for uppercase letters.
letter := 
    - lowercase
    - [AZ]

# Identifier is a rule with a type and an expression. It uses the __text__ 
# macro
identifier <std::string> := (letter | '_') (letter | number | '_')+ {{
        return std::string({% __text__ %});
    }}

# Declaration binds two instances of the identifier rule to use in its 
# expression
declaration <Declaration> := (type: identifier) (name: identifier) ';' {{
        return Declaration({% terms.type %}, {% terms.name %});
    }}
```