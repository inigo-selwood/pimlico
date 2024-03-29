from __future__ import annotations

from copy import copy

import grammar
from grammar import terms
import text
from text import helpers
import tools


class Term:

    def __init__(self):
        self.bounds = (1, 1)
        self.type = ''
        self.binding = ''
        self.ligated = False
        self.predicate = None
    
    @staticmethod
    def decorate(term: grammar.Term, string: str) -> str:
        
        result = ''
        if term.predicate == 'not':
            result += '!'
        elif term.predicate == 'and':
            result += '&'

        result += string

        lower, upper = term.bounds

        if term.bounds == (1, 1):
            pass
        elif term.bounds == (0, None):
            result += '*'
        elif term.bounds == (1, None):
            result += '+'
        elif term.bounds == (0, 1):
            result += '?'
        
        # (n, n) -> {n}
        elif lower is not None and upper is not None and lower == upper:
            result += f'{{{lower}}}'
            
        # (n, None) -> {n:}
        elif lower is not None and upper is None:
            result += f'{{{lower}:}}'
        
        # (None, n) -> {:n}
        elif lower == 0 and upper is not None:
            result += f'{{:{upper}}}'
        
        # (n, m) -> {n:m}
        elif lower is not None and upper is not None:
            result += f'{{{lower}:{upper}}}'

        return result

    @staticmethod
    def parse_bounds(buffer: text.Buffer, errors: tools.ErrorLog):

        # Handle all the built-in shorthand cases
        if buffer.match('*', True):
            return (0, None)
        elif buffer.match('+', True):
            return (1, None)
        elif buffer.match('?', True):
            return (0, 1)
        
        # If there's no specific bound either, return unit count
        elif buffer.match('{{') or not buffer.match('{'):
            return (1, 1)
        
        # Skip the opening bracket
        bounds_position = copy(buffer.position)
        buffer.increment()
        
        # Parse lower bound
        lower_bound = None
        buffer.skip_space()
        if helpers.in_range(buffer.read(), '0', '9'):
            lower_bound = int(helpers.parse_integer(buffer))
        
        # Check for colon
        buffer.skip_space()
        colon_present = buffer.match(':', True)
        
        # Parse upper bound
        upper_bound = None
        buffer.skip_space()
        if helpers.in_range(buffer.read(), '0', '9'):
            upper_bound = int(helpers.parse_integer(buffer))
        
        # Check for a closing bracket
        buffer.skip_space()
        if not buffer.match('}', True):
            errors.add(__name__, 'expected \'}\'', buffer.excerpt())
            return None
        
        # Make sure there was at least one value present
        if lower_bound is None and upper_bound is None:
            errors.add(__name__, 
                    'missing bounds values', 
                    buffer.excerpt(bounds_position))
            return None

        # N instances
        # - term{0} is invalid, since it'll never match
        # - term{1} doesn't need to be specified, one instance is the default
        if (lower_bound is not None 
                and upper_bound is None 
                and not colon_present):

            invalid_bounds = {
                0: 'zero-valued bound',
                1: 'redundant bound'
            }

            if lower_bound in invalid_bounds:
                errors.add(__name__, 
                        invalid_bounds[lower_bound], 
                        buffer.excerpt(bounds_position))
                return None
            
            return (lower_bound, lower_bound)

        # N or more instances
        # - term{0:} is the same as term*
        # - term{1:} is the same as term+
        elif lower_bound is not None and upper_bound is None and colon_present:

            invalid_bounds = {
                0: 'improper bound (should be \'*\')',
                1: 'improper bound (should be \'+\')'
            }

            if lower_bound in invalid_bounds:
                errors.add(__name__, 
                        invalid_bounds[lower_bound], 
                        buffer.excerpt(bounds_position))
                return None

            return (lower_bound, None)

        # Up to N instances
        # - term{:0} makes no sense
        # - term{:1} is the same as term?
        elif lower_bound is None and upper_bound is not None and colon_present:

            invalid_bounds = {
                0: 'zero-valued bound',
                1: 'improper bound (should be \'?\')'
            }

            if upper_bound in invalid_bounds:
                errors.add(__name__, 
                        invalid_bounds[upper_bound], 
                        buffer.excerpt(bounds_position))
                return None
            
            return (0, upper_bound)

        # Between N and M instances
        # - term{n:n} is the same as term{n}
        # - term{n:m} where n > m makes no sense
        # - term{0:n} is the same as term{:n}
        elif (lower_bound is not None
                and upper_bound is not None
                and colon_present):
            
            if lower_bound == upper_bound:
                errors.add(__name__, 
                        f'improper bounds (should be \'{{{lower_bound}}}\')', 
                        buffer.excerpt(bounds_position))
                return None
            
            elif lower_bound > upper_bound:
                errors.add(__name__, 
                        f'invalid bounds ({lower_bound} > {upper_bound})', 
                        buffer.excerpt(bounds_position))
                return None
            
            elif lower_bound == 0:
                errors.add(__name__, 
                        f'improper bounds (should be \'{{:{upper_bound}}}\')', 
                        buffer.excerpt(bounds_position))
                return None
            
            return (lower_bound, upper_bound)
        
        # Failing the above, it's hard to say what went wrong; there's a chance
        # we parsed a _very_ short non-empty embedded expression, but that 
        # should be hard given we're only looking 8 characters ahead
        else:
            errors.add(__name__, 
                    f'invalid bounds',
                    buffer.excerpt(bounds_position))
            return None

    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog):

        # Parse binding
        binding = ''
        binding_position = copy(buffer.position)
        character = buffer.read()
        if (character == '_'
                or helpers.in_range(character, 'a', 'z')
                or helpers.in_range(character, 'A', 'Z')):
            binding = helpers.parse_identifier(buffer)

            buffer.skip_space()
            if not buffer.match(':', True):
                buffer.position = binding_position
                binding = ''
        
        # Parse predicate
        buffer.skip_space()
        predicate = None
        predicate_position = copy(buffer.position)
        if buffer.match('!', consume=True):
            predicate = 'not'
        elif buffer.match('&', consume=True):
            predicate = 'and'

        # Descent delegate
        buffer.skip_space()
        character = buffer.read()
        term = None
        if character == '(':
            term = terms.Sequence.parse(buffer, errors)
        
        elif character == '\'':
            term = terms.Constant.parse(buffer, errors)
        
        elif character == '[':
            term = terms.Range.parse(buffer, errors)
        
        elif buffer.match('_'):
            term = terms.Intrinsic.parse(buffer, errors)
        
        elif (helpers.in_range(character, 'a', 'z')
                or helpers.in_range(character, 'A', 'Z')):
            term = terms.Reference.parse(buffer, errors)
        
        elif character == '`':
            term = terms.Set.parse(buffer, errors)

        else:
            errors.add(__name__, 'expected a term', buffer.excerpt())
            return None

        if not term:
            return None

        # Parse bounds
        buffer.skip_space()
        term.bounds = Term.parse_bounds(buffer, errors)
        if not term.bounds:
            return None
        
        lower_bound, _ = term.bounds
        if predicate is not None and lower_bound == 0:
            errors.add(__name__, 
                    'predicate given for an optional term',
                    buffer.excerpt(predicate_position))
            return None
        
        term.predicate = predicate
        term.binding = binding
        return term

    @staticmethod
    def present(buffer: text.Buffer) -> bool:

        character = buffer.read()
        return (character == '!'
                or character == '&'
                or character == '\''
                or character == '['
                or helpers.in_range(character, 'a', 'z')
                or helpers.in_range(character, 'A', 'Z')
                or character == '_'
                or character == '('
                or character == '`')

    @staticmethod
    def greedy_parser(match_function: callable) -> tuple:

        def functor(self, buffer: text.Buffer):

            position = copy(buffer.position)

            text = ''
            match_count = 0
            lower_bound, upper_bound = self.bounds
            while True:

                if buffer.finished():
                    break

                term_match, match_text = match_function(self, buffer)
                if term_match:
                    match_count += 1
                    text += match_text
                else:
                    break
            
                if upper_bound is not None and match_count == upper_bound:
                    break

            success = match_count >= lower_bound

            if self.predicate == 'not':
                buffer.position = position
                return (not success, '')
            elif self.predicate == 'and':
                buffer.position = position
                return (success, '')
            
            elif not success:
                buffer.position = position
                return (False, '') 
            
            return (True, text)
        
        return functor
    
    def link_rules(self, 
            rules: dict, 
            buffer: text.Buffer, 
            errors: tools.ErrorLog,
            parent: grammar.Rule) -> bool:
        return True
