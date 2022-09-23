from __future__ import annotations

from perivale import Buffer

from grammar import Term


class Range(Term):

    def __init__(self, bounds: tuple):
        super().__init__()
        self.bounds = bounds

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Range:
        position = buffer.copy_position()

        if not buffer.match("[", consume=True):
            raise Exception("expected range")
        
        values = [''] * 2
        for index in range(2):

            buffer.skip_space()
            if buffer.finished() or buffer.match("]"):
                errors.append(buffer.error("expected range value"))
                return None
            if buffer.match("' '", consume=True):
                values[index] = " "
            elif buffer.match(r"\]", consume=True):
                values[index] = "]"
            else:
                values[index] = buffer.read(consume=True)

        buffer.skip_space()
        if not buffer.match("]", consume=True):
            errors.append(buffer.error("expected ']'"))
            return None

        bounds = tuple(values)

        lower, upper = bounds
        if lower >= upper:
            errors.append(buffer.error("illogical range bounds", position))
            return None
        
        result = Range(bounds)
        return result