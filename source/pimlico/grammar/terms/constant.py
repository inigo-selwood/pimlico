from __future__ import annotations

from perivale import Buffer, ParseException

from grammar import Term


class Constant(Term):

    def __init__(self, text: str):
        super().__init__()
        self.text = text

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Constant:
        position = buffer.copy_position()
        
        text = ""
        try:
            text = buffer.parse_bounded_text(("'", "'"), consume=True)
        except ParseException as error:
            errors.append(error)
            return None

        text = text[1:-1]
        if not text:
            errors.append(buffer.error("empty constant", position))
            return None
        
        result = Constant(text)
        return result