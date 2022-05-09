import grammar
from text.parse_bounded_text import parse_bounded_text
from text import ParseBuffer, ErrorBuffer
from utilities import in_range


class Program:

    domain = 'grammar.program'

    def __init__(self, rules: dict, includes: list):
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

        includes = []
        rules = {}
        while True:

            if buffer.finished():
                break
        
            # Check the indentation is correct
            elif buffer.line_indentation() != 0:
                buffer.skip_space()
                errors.add(domain, 
                        'unexpected indentation increase', 
                        buffer.position)
                return None

            # Handle inclusion macros
            elif buffer.match('.include', True):

                buffer.skip_space()
                if not buffer.match('('):
                    errors.add(domain, 'expected \'(\'', buffer.position)
                
                inclusion = parse_bounded_text(buffer, 
                        errors, 
                        '(', 
                        ')', 
                        permit_newlines=False)
                if not inclusion:
                    return None
                
                includes.append(inclusion)
                buffer.skip_space(include_newlines=True)
                continue

            # Check there's a rule name present
            character = buffer.read()
            if (character != '_'
                    and not in_range(character, 'a', 'z')
                    and not in_range(character, 'A', 'Z')):
                errors.add(domain, 'expected a rule', buffer.position)
                return None 

            # Parse the rule
            rule = grammar.Rule.parse(buffer, errors)
            if not rule:
                return None
            elif rule.name in rules:
                errors.add(domain, 
                        f'duplicate rule \'{rule.name}\'', 
                        rule.position)
                errors.add(domain,
                        'first declared here',
                        rules[rule.name].position)
                return None
            rules[rule.name] = rule
            
            # Check for trailing garbage
            buffer.skip_space()
            if not buffer.finished() and not buffer.match('\n'):
                errors.add(domain,
                        'expected a newline',
                        buffer.position)
                return None
            
            buffer.skip_space(include_newlines=True)
        
        return Program(rules, includes)

