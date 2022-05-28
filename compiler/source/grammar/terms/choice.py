from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
from grammar import terms
import text
import tools

from ..term import Term


class Choice(Term):

    def __init__(self, terms: dict, position: text.Position):
        super(Choice, self).__init__()

        self.terms = terms
        self.position = position
        self.type = 'choice'

        context = sha256()
        for hash in sorted(terms.keys()):
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        result = ''

        # Put brackets around the choice if it has an instance bound
        enclosed = False
        if self.bounds != (1, 1):
            result += '('
            enclosed = True

        index = 0
        count = len(self.terms)
        for term in self.terms.values():

            # If it's a sequence inside a choice, put backets around it
            term_enclosed = False
            if term.type == 'sequence':
                result += '('
                term_enclosed = True

            # Add the term and a closing bracket
            result += term.__str__()
            if term_enclosed:
                result += ')'
                result += super(terms.Sequence, term).__str__()
            
            # Add a choice pipe
            if index + 1 < count:
                result += ' | '
            index += 1
        
        # Close the bracket, if there was one
        if enclosed:
            result += ')'
        instances = super(Choice, self).__str__()
        
        return f'{result}{instances}'
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Choice:
        
        position = copy(buffer.position)

        options = {}
        valid = True
        while True:

            # Parse term
            term = grammar.Term.parse(buffer, errors)
            if not term:
                return None
            
            elif term.hash in options:
                errors.add(__name__, 
                        'duplicate option', 
                        buffer.excerpt(term.position))
                valid = False
            
            else:
                options[term.hash] = term

            # Check for a pipe character, stop parsing if we don't find one
            buffer.skip_space()
            pipe_position = copy(buffer.position)
            if not buffer.match('|', consume=True):
                break

            # Look ahead to see if there are more pipe characters; report each
            # one as an empty option
            while True:
                buffer.skip_space()
                new_pipe_position = copy(buffer.position)
                if buffer.match('|', consume=True):
                    errors.add(__name__, 
                            'empty option', 
                            buffer.excerpt(pipe_position))
                    valid = False
                else:
                    break

                pipe_position = new_pipe_position

            # Check buffer not finished mid-choice
            if buffer.finished():
                errors.add(__name__, 
                        'unexpected end-of-file', 
                        buffer.excerpt())
                return None
            
            # Make sure newline not present
            elif buffer.match('\n'):
                errors.add(__name__, 
                        'unexpected newline', 
                        buffer.excerpt())
                return None

        if not valid:
            return None
        
        # Return first element if only one found
        elif len(options) == 1:
            return list(options.values())[0]

        return Choice(options, position)
    
    @Term.greedy_parser
    def match(self, buffer: text.Buffer) -> tuple:
        
        for term in self.terms.values():
            position = copy(buffer.position)
            result = term.match(buffer)
            
            term_match, _ = result
            if term_match:
                return result
            
            buffer.position = position
        
        return (False, '')
    
    def link_rules(self, 
            rules: dict, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog,
            parent: grammar.Rule) -> bool:
        
        success = True
        for term in self.terms.values():

            term_link_result = term.link_rules(rules, buffer, errors, parent)
            success = success and term_link_result
        
        return success
