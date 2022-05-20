from copy import copy
from utilities import format_c, indent_text
from text import ParseBuffer, ErrorBuffer, parse_bounded_text
import grammar


class Production:
    
    domain = 'grammar.production'

    def __init__(self, term, terminal: bool, expression: str):
        self.term = term
        self.terminal = terminal
        self.expression = expression
    
    def __str__(self):
        if not self.expression:
            return self.term.__str__()
        
        indented_text = indent_text(self.expression, 2)
        return (f'{self.term.__str__()} {{'
                f'\n        {indented_text}'
                '    }')
    
    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parse a production expression

        Productions consist of a sequence of terms, and an optional expression:

        ```
        term0 term1 ... termN [{ // Expression }]
        ```

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at an expected production
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        production: Production
            the parsed production, or None if the parsing failed
        '''

        domain = f'{Production.domain}:parse'

        if not grammar.Term.present(buffer):
            errors.add(domain, 'expected a term', buffer.position, buffer)
            return None
        
        term = grammar.terms.Sequence.parse(buffer, errors, True)
        if not term:
            return None
        
        # Parse embedded expression
        expression = ''
        buffer.skip_space()
        expression_position = copy(buffer.position)
        if buffer.match('{'):

            expression = parse_bounded_text(buffer, errors, '{', '}')
            if expression is None:
                return None
            
            expression = expression.strip()
            if not expression:
                errors.add(domain, 
                        'empty expression', 
                        expression_position, 
                        buffer)
                return None
        
            expression = format_c(expression)
        
        # Check if this production is terminal by looking for references
        terminal = True
        stack = [term]
        while True:

            if not stack:
                break
        
            head = stack[-1]
            stack = stack[:-1]
            if head.type == 'choice':
                stack.extend(head.values.values())
            elif head.type == 'sequene':
                stack.extend(head.values)
            elif head.type == 'reference':
                terminal = False
                break
        
        return Production(term, terminal, expression)
    
    def link_references(self, 
            rules: dict, 
            parent_rule,
            buffer: ParseBuffer, 
            errors: ErrorBuffer) -> bool:
        
        ''' Links rules to references in this production's terms

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
        parent_rule: Rule
            the rule in which this term is defined
        buffer: ParseBuffer
            the buffer used for parsing
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        success: bool
            whether or not the linking succeeded
        '''

        return self.term.link_references(rules, parent_rule, buffer, errors)