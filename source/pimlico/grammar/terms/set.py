from __future__ import annotations

from perivale import Buffer, ParseError
from perivale.helpers import parse_escape_code

from pimlico.grammar import Term


class Set(Term):

    def __init__(self, values: str):
        super().__init__()
        self.values = values

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Set | None:

        set_position = buffer.copy_position()

        # Check constant present
        if not buffer.match("`", consume=True):
            raise Exception("expected \"`\"")

        values = ""
        success = True
        while True:

            # Break when finished
            if buffer.match("`") or buffer.finished() or buffer.match("\n"):
                break
        
            # Handle escape codes
            elif buffer.match("\\"):
                escape_codes = {
                    "\\": "\\",
                    "`": "`",
                    "n": "\n",
                    "r": "\r",
                    "t": "\t",
                }

                code_position = buffer.copy_position()
                code = parse_escape_code(buffer, 
                        escape_codes, 
                        errors, 
                        consume=True)
                if not code:
                    success = False
                    continue

                if code in values:
                    error = ParseError()
                    error.add_excerpt("duplicate value in set", 
                            buffer.excerpt(code_position))
                    errors.append(error)
                    success = False
                    continue

                values += code
            
            # Add anything else directly
            else:

                value_position = buffer.copy_position()
                value = buffer.read(consume=True)
                if value in values:
                    error = ParseError()
                    error.add_excerpt("duplicate value in set", 
                            buffer.excerpt(value_position))
                    errors.append(error)
                    success = False
                    continue
                
                values += value
        
        if not buffer.match("`", consume=True):
            error = ParseError()
            error.add_excerpt("expected \"`\"", 
                    buffer.excerpt())
            error.add_excerpt("to match this",
                    buffer.excerpt(set_position))
            errors.append(error)
            return None
        
        elif not success:
            return None
        
        # Check non-empty
        elif not values:
            error = ParseError()
            error.add_excerpt("empty set", 
                    buffer.excerpt(set_position))
            errors.append(error)
            return None
        
        return Set(values)
    
    def serialize(self) -> str:
        values = self.values.replace("\\", "\\\\")

        replacements = {
            "\n": "\\n",
            "\r": "\\r",
            "\t": "\\t",
            "`": "\\`",
        }
        for symbol, replacement in replacements.items():
            values = values.replace(symbol, replacement)

        return f"`{values}`"


