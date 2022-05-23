from __future__ import annotations

import grammar
from grammar import terms
import text
import tools


class Schema:

    def __init__(self):
        self.rules = {}

    @staticmethod
    def parse(self, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Schema:
        
        pass
