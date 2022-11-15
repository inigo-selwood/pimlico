from __future__ import annotations

from perivale import Buffer, ParseError
from perivale.helpers import parse_escape_code

from pimlico.grammar import Term


class Constant(Term):

    def __init__(self, text: str):
        super().__init__()
        self.text = text

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Constant | None:

        constant_position = buffer.copy_position()

        # Check constant present
        if not buffer.match("'", consume=True):
            raise Exception("expected \"'\"")

        text = ""
        success = True
        while True:

            # Break when finished
            if buffer.match("'") or buffer.finished() or buffer.match("\n"):
                break
        
            # Handle escape codes
            elif buffer.match("\\"):
                escape_codes = {
                    "\'": "\'",
                    "\\": "\\",
                    "n": "\n",
                    "r": "\r",
                    "t": "\t",
                }
                code = parse_escape_code(buffer, 
                        escape_codes, 
                        errors, 
                        consume=True)
                if code is None:
                    success = False
                    continue
                text += code
            
            # Add anything else directly
            else:
                text += buffer.read(consume=True)
        
        if not buffer.match("'", consume=True):
            error = ParseError()
            error.add_excerpt("expected \"'\"", 
                    buffer.excerpt())
            error.add_excerpt("to match this",
                    buffer.excerpt(constant_position))
            errors.append(error)
            return None
        
        elif not success:
            return None
        
        # Check non-empty
        elif not text:
            error = ParseError()
            error.add_excerpt("empty constant", 
                    buffer.excerpt(constant_position))
            errors.append(error)
            return None
        
        return Constant(text)
    
    def serialize(self) -> str:
        text = self.text.replace("\\", "\\\\")

        replacements = {
            "'": "\\'",
            "\n": "\\n",
            "\r": "\\r",
            "\t": "\\t",
        }
        for symbol, replacement in replacements.items():
            text = text.replace(symbol, replacement)

        return f"'{text}'"