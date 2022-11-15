from __future__ import annotations

from perivale import Buffer, ParseError
from perivale.helpers import (identifier_present, 
        parse_identifier,
        integer_present,
        parse_integer)

from pimlico import grammar


class Term:

    def __init__(self):
        self.binding = None
        self.hint = (1, 1)
        self.predicate = None
    
    def __str__(self) -> str:
        binding = f"{self.binding}: " if self.binding else ""

        hint = Term.serialize_hint(self.hint)
        
        symbols = {
            "and": "&",
            "not": "!",
        }
        predicate = ""
        if self.predicate in symbols:
            predicate = symbols[self.predicate]
        
        result = self.serialize()
        multi_term = (isinstance(self, grammar.terms.Choice) 
                or isinstance(self, grammar.terms.Sequence))
        if multi_term and (binding or hint or predicate):
            result = f"({result})"

        return f"{binding}{predicate}{result}{hint}"
    
    @staticmethod
    def present(buffer: Buffer) -> bool:
        character = buffer.read()
        return (character 
                and (character in "!&('[`" 
                    or identifier_present(buffer)))

    @staticmethod
    def parse(buffer: Buffer, errors: list, root: bool = False) -> Term | None:

        term_position = buffer.copy_position()

        # Handle bindings
        binding = None
        if identifier_present(buffer):
            binding = parse_identifier(buffer, consume=True)

            buffer.skip_space()
            if not buffer.match(":", consume=True):
                buffer.position = term_position
                binding = None

        # Check for predicate
        buffer.skip_space()
        predicate = None
        if buffer.match("!", consume=True):
            predicate = "not"
        elif buffer.match("&", consume=True):
            predicate = "and"
        
        # Check predicate and binding not both present
        if predicate and binding:
            error = ParseError()
            error.add_excerpt("cannot both bind and predicate a term",
                    buffer.excerpt(term_position))
            errors.append(error)
            return None
        
        # Delegate term parsing
        buffer.skip_space()
        term = None
        if root or buffer.match("("):
            term = grammar.terms.Sequence.parse(buffer, errors)
        elif buffer.match("'"):
            term = grammar.terms.Constant.parse(buffer, errors)
        elif buffer.match("["):
            term = grammar.terms.Range.parse(buffer, errors)
        elif identifier_present(buffer):
            term = grammar.terms.Reference.parse(buffer, errors)
        elif buffer.match("`"):
            term = grammar.terms.Set.parse(buffer, errors)
        else:
            error = ParseError()
            error.add_excerpt("expected a term",
                    buffer.excerpt(term_position))
            errors.append(error)
            return None
        
        if term is None:
            return None
        
        buffer.skip_space()
        hint = Term.parse_hint(buffer, errors)
        if hint is None:
            return None
        
        term.binding = binding
        term.predicate = predicate
        term.hint = hint

        return term
    
    @staticmethod
    def parse_hint(buffer: Buffer, errors: list) -> tuple | None:

        # Handle built-ins
        if buffer.match("?", consume=True):
            return (0, 1)
        elif buffer.match("*", consume=True):
            return (0, -1)
        elif buffer.match("+", consume=True):
            return (1, -1)
        
        # Check for specific hint
        hint_position = buffer.copy_position()
        if not buffer.match("<", consume=True):
            return (1, 1)
        
        lower = None
        upper = None
        colon_present = False

        # Parse lower bound
        buffer.skip_space()
        if integer_present(buffer):
            lower = parse_integer(buffer, consume=True)
        
        # Check for colon
        buffer.skip_space()
        if buffer.match(":", consume=True):
            colon_present = True
        
        # Parse upper bound
        buffer.skip_space()
        if integer_present(buffer):
            upper = parse_integer(buffer, consume=True)
        
        # Check for closing brace
        buffer.skip_space()
        if not buffer.match(">", consume=True):
            error = ParseError()
            error.add_excerpt("expected '>'", buffer.excerpt())
            error.add_excerpt("to match this", buffer.excerpt(hint_position))
            errors.append(error)
            return None
        
        # <n>
        if lower is not None and not colon_present and upper is None:
            if lower in [0, 1]:
                error = ParseError()
                error_type = "redundant" if lower == 1 else "illogical"
                error.add_excerpt(f"{error_type} hint",
                        buffer.excerpt(hint_position))
                errors.append(error)
                return None
            return (lower, lower)
        
        # <n:>
        elif lower is not None and colon_present and upper is None:
            return (lower, -1)
        
        # <:n>
        elif lower is None and colon_present and upper is not None:
            if upper == 0:
                error = ParseError()
                error.add_excerpt(f"illogical hint",
                        buffer.excerpt(hint_position))
                errors.append(error)
                return None
            return (0, upper)
        
        # <n:m>
        elif lower is not None and colon_present and upper is not None:
            if lower >= upper:
                error = ParseError()
                error.add_excerpt(f"illogical hint",
                        buffer.excerpt(hint_position))
                errors.append(error)
                return None
            return (lower, upper)
        
        # Failing those formats
        error = ParseError()
        error.add_excerpt(f"invalid hint",
                buffer.excerpt(hint_position))
        errors.append(error)
        return None
    
    @staticmethod
    def serialize_hint(hint: tuple) -> str:
        if hint == (1, 1):
            return ""
        
        elif hint == (0, 1):
            return "?"
        elif hint == (0, -1):
            return "*"
        elif hint == (1, -1):
            return "+"
        
        lower, upper = hint
        if lower == upper:
            return f"<{lower}>"
        elif upper == -1:
            return f"<{lower}:>"
        elif lower == 0:
            return f"<:{upper}>"
        else:
            return f"<{lower}:{upper}>"
    
    def serialize(self) -> str:
        pass