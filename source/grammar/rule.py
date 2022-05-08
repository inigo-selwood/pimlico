from copy import copy

from text import (Position,
        ParseBuffer,
        ErrorBuffer,
        parse_bounded_text,
        parse_identifier)
import grammar


class Rule:

    domain = 'grammar.rule'

    def __init__(self, 
            name: str, 
            type: str, 
            productions: list, 
            position: Position):
        
        self.name = name
        self.type = type
        self.productions = productions
        self.position = position

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a rule

        There are two forms of rules. In the first case, there's just one 
        inline value:

        ```
        rule <type> := term {}
        ```

        When you want a rule to have multiple productions, they're indented by
        1 tab, and prefixed with a hyphen:

        ```
        rule <type> :=
            - term1 {}
            - term2 {}
        ```

        Arguments
        ---------
        buffer: ParseBuffer
            the buffer where the rule is present
        errors: ErrorBuffer
            for reporting errors
        
        Returns
        -------
        rule: Rule
            the parsed rule
        '''

        domain = f'{Rule.domain}:parse'
        start_position = copy(buffer.position)

        name = parse_identifier(buffer)
        assert name, 'expected a name'

        buffer.skip_space()
        type = ''
        if buffer.match('<'):
            type = parse_bounded_text(buffer, errors, '<', '>')
            if not type:
                return None

        buffer.skip_space()
        if not buffer.match(':=', True):
            errors.add(domain, 'expected \':=\'', buffer.position)
        
        # There are 2 types of rule forms; one has a single production on the 
        # same line, and the other has a list of them on newlines, indented by
        # 1 tab, and prefixed with a hyphen `-`.
        # This should be the inline case
        productions = []
        buffer.skip_space()
        if grammar.Term.present(buffer):
            production = grammar.Production.parse(buffer, errors)
            if not production:
                return None
            
            buffer.skip_space()
            if not buffer.finished() and not buffer.match('\n'):
                errors.add(domain, 'expected newline', buffer.position)
                return None
        
        # If there isn't an inline term, we presume there's multiple
        # bullet-point indented ones
        else:
            
            while True:

                buffer.skip_space(True)
                if (not buffer.line_indentation() == 4
                        or not buffer.match('-', True)):
                    break
                
                buffer.skip_space()
                if not grammar.Term.present(buffer):
                    errors.add(domain, 'expected a term', buffer.position)
                    return None
                
                production = grammar.Production.parse(buffer, errors)
                if not production:
                    return None
                
                productions.append(production)
            
            if not productions:
                errors.add(domain, 'expected a production', buffer.position)
                return None
            
        return Rule(name, type, productions, start_position)


