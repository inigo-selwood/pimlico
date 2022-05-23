from __future__ import annotations

import grammar
from grammar import terms
from text import helpers
import text
import tools


class Schema:

    def __init__(self, rules: dict, includes: list):
        self.rules = rules
        self.includes = includes

    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Schema:

        # Move to start, check not empty
        buffer.skip_space(include_newlines=True)
        if buffer.finished():
            errors.add(__name__, 'empty text')
            return None

        includes = []
        rules = {}
        while True:

            # Skip space, comments
            while True:
                buffer.skip_space(include_newlines=True)
                
                if not buffer.match('#'):
                    break
                buffer.skip_line()

            # Move to next rule
            buffer.skip_space(include_newlines=True)
            if buffer.finished():
                break
        
            # Check the indentation is correct
            elif buffer.line_indentation() != 0:
                buffer.skip_space()
                errors.add(__name__, 
                        'invalid indentation', 
                        buffer.excerpt())
                return None

            # Handle inclusion macros
            elif buffer.match('.include', True):

                buffer.skip_space()
                if not buffer.match('('):
                    errors.add(__name__, 
                            'expected \'(\'', 
                            buffer.excerpt())
                
                inclusion = helpers.parse_expression(buffer, 
                        ('(', ')'), 
                        errors, 
                        permit_newlines=False)
                if not inclusion:
                    return None
                
                includes.append(inclusion)
                buffer.skip_space(include_newlines=True)
                continue

            # Check there's a rule name present
            character = buffer.read()
            if (character != '_'
                    and not helpers.in_range(character, 'a', 'z')
                    and not helpers.in_range(character, 'A', 'Z')):
                errors.add(__name__, 'expected a rule', buffer.excerpt())
                return None 

            # Parse the rule
            rule = grammar.Rule.parse(buffer, errors)
            if not rule:
                return None
            
            # Make sure the rule isn't already defined elsewhere
            elif rule.name in rules:

                errors.add(__name__, 
                        f'duplicate rule', 
                        buffer.excerpt(rule.position))
                errors.add(__name__,
                        'first defined here',
                        buffer.excerpt(rules[rule.name].position))
                return None
            rules[rule.name] = rule
            
            # Check for trailing garbage
            buffer.skip_space()
            if not buffer.finished() and not buffer.match('\n'):
                errors.add(__name__,
                        'expected newline',
                        buffer.excerpt())
                return None
            
        # # Emplace rule references
        # link_success = True
        # for rule in rules.values():
        #     link_success = (link_success
        #             and rule.link_references(rules, buffer, errors))
        
        # if not link_success:
        #     return None
        
        return Schema(rules, includes)