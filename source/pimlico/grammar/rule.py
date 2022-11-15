from __future__ import annotations

from perivale import Buffer, ParseError
from perivale.helpers import (identifier_present, 
        parse_identifier, 
        parse_expression)

from .terms.sequence import Sequence
from .term import Term


class Rule:

    def __init__(self, name: str, productions: list):
        self.name = name
        self.productions = productions

    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Rule:

        # Check for rule name
        if not identifier_present:
            raise ValueError("expected a rule")
        
        rule_position = buffer.copy_position()
        name = parse_identifier(buffer, consume=True)

        # Parse productions
        productions = []
        while True:

            buffer.skip_space(include_newlines=True)
            indentation = buffer.line_indentation()
            production_position = buffer.copy_position()
            if buffer.finished() or not buffer.match(":=", consume=True):
                break
            
            # Check the indentation is valid
            if indentation != 4:
                error = ParseError()
                error.add_excerpt("invalid indentation", 
                        buffer.excerpt(production_position))
                errors.append(error)
                return None

            # Check a term is present
            buffer.skip_space()
            if not Term.present(buffer):
                error = ParseError()
                error.add_excerpt("expected a term", buffer.excerpt())
                errors.append(error)
                return None

            # Parse production statement
            seqeunce = Sequence.parse(buffer, errors)
            if seqeunce is None:
                return None
            
            # Parse embedded statement
            expression = None
            buffer.skip_space()
            if buffer.match("{"):
                
                escape_codes = {
                    "\\'": "'",
                    "\\\"": "\"",
                    "\\\\": "\\",
                    "\\n": "\n",
                    "\\t": "\t",
                    "\\r": "\r",
                }

                expression_position = buffer.copy_position()
                expression = parse_expression(buffer, 
                        ("{", "}"), 
                        errors, 
                        permit_newlines=True, 
                        escape_codes=escape_codes)
                if expression is None:
                    return None

                expression = expression.strip("{}").strip()
                if not expression:
                    error = ParseError()
                    error.add_excerpt("empty expression", 
                            buffer.excerpt(expression_position))
                    errors.append(error)
                    return None

            productions.append((seqeunce, expression))
        
        if not productions:
            error = ParseError()
            error.add_excerpt("a rule expects productions", 
                    buffer.excerpt(rule_position))
            errors.append(error)
            return None

        return Rule(name, productions)