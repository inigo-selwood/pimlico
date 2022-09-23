from __future__ import annotations

from perivale import Buffer, ParseException

from grammar import Term


class Set(Term):

    def __init__(self, values: str):
        super().__init__()
        self.values = values

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Set:
        position = buffer.copy_position()
        
        values = ""
        try:
            values = buffer.parse_bounded_text(("`", "`"), consume=True)
        except ParseException as error:
            errors.append(error)
            return None

        values = values[1:-1]
        if not values:
            errors.append(buffer.error("empty set", position))
            return None
        
        stack = ""
        duplicates = ""
        for value in values:
            if value in stack:
                duplicates += value
            stack += value
        
        if duplicates:
            characters = [f"'{character}'" for character in duplicates]
            tokens = ", ".join(characters)
            message = f"duplicate character(s) in set: {tokens}"
            errors.append(buffer.error(message, position))
            return None
        
        result = Set(values)
        return result