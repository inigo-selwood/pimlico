from __future__ import annotations

from perivale import Buffer, Set

from grammar import Term


class Reference(Term):

    def __init__(self, text: str):
        super().__init__()
        self.text = text
        self.rule = None

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Reference:
        text = buffer.parse_set(Set.SNAKE, consume=True)
        if not text:
            raise Exception("expected a reference")
        
        result = Reference(text)
        return result
    
    def bind_references(self, 
            buffer: Buffer, 
            rules: dict, 
            errors: list) -> bool:
        
        if self.text not in rules:
            errors.append(buffer.error("undefined reference", self.position))
            return False
        
        self.rule = rules[self.text]
        return True