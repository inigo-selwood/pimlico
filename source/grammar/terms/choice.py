from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
import text
import tools

from ..term import Term


class Choice(Term):

    def __init__(self, choice_terms: list, position: text.Position):
        self.terms = choice_terms
        self.position = position
        self.type = 'choice'

        context = sha256()
        for hash in sorted(choice_terms.keys()):
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()
    
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