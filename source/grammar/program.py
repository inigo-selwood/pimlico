import grammar
from text import ParseBuffer, ErrorBuffer
from utilities import in_range


class Program:

    domain = 'grammar.program'

    def __init__(self, rules: dict):
        self.rules = rules

    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a grammar

        Arguments
        ---------
        buffer: ParseBuffer
            a buffer containing the grammar to parse
        errors: ErrorBuffer
            for reporting errors
        
        Returns
        -------
        grammar: Program
            the parsed grammar object
        '''

        domain = f'{Program.domain}:parse'

        buffer.skip_space(include_newlines=True)
        if buffer.finished():
            errors.add(domain, 'empty text', buffer.position)
            return None

        rules = {}
        while True:

            if buffer.finished():
                break
        
            if buffer.line_indentation() != 0:
                buffer.skip_space()
                errors.add(domain, 
                        'unexpected indentation increase', 
                        buffer.position)
                return None

            character = buffer.read()
            if (character != '_'
                    and not in_range(character, 'a', 'z')
                    and not in_range(character, 'A', 'Z')
                    and not in_range(character, '0', '9')):
                errors.add(domain, 'expected a rule', buffer.position)
                return None 

            rule = grammar.Rule.parse(buffer, errors)
            if not rule:
                return None
            if rule.name in rules:
                errors.add(domain, 
                        f'duplicate rule \'{rule.name}\'', 
                        rule.position)
                errors.add(domain,
                        'first declared here',
                        rules[rule.name].position)
                return None
            rules[rule.name] = rule
            
            buffer.skip_space()
            if not buffer.finished() and not buffer.match('\n'):
                errors.add(domain,
                        'expected a newline',
                        buffer.position)
                return None
            
            buffer.skip_space(include_newlines=True)
        
        return Program(rules)

