from __future__ import annotations

import subprocess
from copy import copy
import json

import text
from text import helpers
import tools
import grammar
from grammar import terms


class Production:

    def __init__(self, terms: list, expression: str):
        self.terms = terms
        self.expression = expression
    
    def __str__(self):
        result = ''

        index = 0
        count = len(self.terms)
        for term in self.terms:

            result += term.__str__()
            if (index + 1) < count:
                result += ' '
            index += 1
            
        if self.expression:
            lines = self.expression.split('\n')
            indentation = ' ' * 8
            indented_text = f'\n{indentation}'.join(lines)

            result += (' {{'
                    f'\n        {indented_text}'
                    '\n    }}')

        return result

    @staticmethod
    def parse(buffer: text.Buffer, 
            errors: tools.ErrorLog) -> grammar.Production:
        
        # Check production is present
        if not grammar.Term.present(buffer):
            errors.add(__name__, 'expected a term', buffer.excerpt())
            return None
        
        values = []
        valid = True
        while True:

            # Parse term
            term = terms.Choice.parse(buffer, errors)
            if not term:
                return None
            
            # Check value not duplicated
            elif values and term.hash == values[-1].hash:
                errors.add(__name__, 
                        'redundant (instance hint)', 
                        buffer.excerpt(term.position,))
                valid = False
            
            else:
                values.append(term)

            # Break when a newline, expression, or enclosure are reached
            buffer.skip_space()
            if (buffer.finished()
                    or buffer.match('\n')
                    or buffer.match('{')
                    or buffer.match(')')):
                break

        if not valid:
            return None
                
        # Check for, and parse an embedded expression
        expression = ''
        buffer.skip_space()
        expression_position = copy(buffer.position)
        if buffer.match('{{'):

            # Parse the expression, check it was okay
            expression = helpers.parse_expression(buffer, 
                    ('{{', '}}'), 
                    errors, 
                    permit_newlines=True)
            if expression is None:
                return None
            
            # Make sure the expression isn't empty
            expression = expression[2:-2].strip()
            if not expression:
                errors.add(__name__, 
                        'empty expression', 
                        buffer.excerpt(expression_position))
                return None
            
            # Format the expression
            try:
                flags = {
                    'BasedOnStyle': 'llvm',
                    'AllowShortFunctionsOnASingleLine': False,
                    'IndentWidth': 4,
                }

                style_flags = json.dumps(flags, separators=(',', ':'))

                command = [
                    'clang-format',
                    f'-style={style_flags}',
                ]

                process = subprocess.run(command, 
                        check=True,
                        input=expression.encode(), 
                        capture_output=True)
                expression = process.stdout.decode('utf-8')

            except FileNotFoundError as error:
                pass
        
        return Production(values, expression)