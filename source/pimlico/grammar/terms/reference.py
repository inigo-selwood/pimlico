from __future__ import annotations

from perivale import Buffer
from perivale.helpers import parse_identifier, identifier_present

from pimlico.grammar import Term


class Reference(Term):

    def __init__(self, text: str):
        super().__init__()
        self.text = text

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Reference | None:
        if not identifier_present(buffer):
            raise Exception("expected a reference")
        return Reference(parse_identifier(buffer, consume=True))
    
    def serialize(self) -> str:
        return self.text
        
