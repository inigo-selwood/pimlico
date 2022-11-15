from __future__ import annotations

from perivale import Buffer, ParseError
from perivale.helpers import parse_escape_code

from pimlico.grammar import Term


class Range(Term):

    def __init__(self, bounds: tuple):
        self.bounds = bounds
    
    def __str__(self) -> str:
        return self.serialize()
    
    def serialize(self) -> str:
        lower, upper = self.bounds
        return f"[{lower}{upper}]"

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Range | None:

        def parse_bound() -> str:

            # Check there's a bound value present
            if (buffer.finished() or buffer.match("]") or buffer.match("\n")):
                error = ParseError()
                error.add_excerpt("expected range bound value", 
                        buffer.excerpt())
                errors.append(error)
                return None

            # Handle literal spaces
            elif buffer.match("' '", consume=True):
                return " "
        
            # Handle escape codes
            elif buffer.match("\\"):
                escape_codes = {
                    "]": "]",
                    "\\": "\\",
                }
                return parse_escape_code(buffer, 
                        escape_codes, 
                        errors, 
                        consume=True)
            
            return buffer.read(consume=True)

        # Check range present
        range_position = buffer.copy_position()
        if not buffer.match("[", consume=True):
            raise Exception("expected '['")
        
        # Parse bounds
        bounds = [None] * 2
        for index in range(2):
            buffer.skip_space()
            bound = parse_bound()
            if bound is None:
                break
            bounds[index] = bound
        
        # Check two values present
        if None in bounds:
            return None
        
        # Check for closing bracket
        buffer.skip_space()
        if not buffer.match("]", consume=True):
            error = ParseError()
            error.add_excerpt("expected ']'", 
                    buffer.excerpt())
            error.add_excerpt("to match this", 
                    buffer.excerpt(range_position))
            errors.append(error)
            return None

        # Check range logical
        lower, upper = bounds
        lower_index, upper_index = [ord(bound) for bound in bounds]
        if lower_index >= upper_index:
            error = ParseError()
            error.add_excerpt(f"illogical range bounds ('{lower}' >= '{upper}')", 
                    buffer.excerpt(range_position))
            errors.append(error)
            return None

        return Range(tuple(bounds))