from copy import copy

from utilities import in_range
from text import (ParseBuffer, 
        ErrorBuffer, 
        parse_bounded_text, 
        parse_identifier, 
        parse_integer)

import grammar


class Term:

    domain = 'grammar.term'

    def __init__(self):
        pass

    @staticmethod
    def parse_bounds(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a set of bounds

        Certain common bound values have shorthand:

        - term+ (one or more terms)
        - term* (zero or more terms)
        - term? (an optional term)

        Specific bounds are more versatile:

        - term{n} (n terms)
        - term{n:} (n or more terms)
        - term{:n} (up to n terms)
        - term{n:m} (between n and m terms)

        Note: in the absence of a bound value, the default bounds (1, 1) will
        be returned

        Arguments
        ---------
        buffer: ParseBuffer
            the parse context
        errors: ErrorBuffer
            for reporting errors
        
        Returns
        -------
        bounds: [int, int]
            the parsed bounds, or None if an error was encountered. A value of
            -1 indicates an infinite bound. ie: (1, -1) means "one or more"
        '''

        # Handle all the built-in shorthand cases
        if buffer.match('*', True):
            return (0, -1)
        elif buffer.match('+', True):
            return (1, -1)
        elif buffer.match('?', True):
            return (0, 1)
        
        # Check to see if there's a specific bound, presuming it's less than
        # 12 characters long -- we do this check to not accidentally consume
        # a production expression, mistaking it for a bound. 
        elif not buffer.match('{') or not buffer.seek('}', limit=8):
            return (1, 1)
        
        section = f'{Term.domain}.bounds:parse'

        # Skip the opening bracket
        bounds_position = copy(buffer.position)
        buffer.increment()
        
        # Parse lower bound
        lower_bound = None
        buffer.skip_space()
        if in_range(buffer.read(), '0', '9'):
            lower_bound = parse_integer(buffer)
        
        # Check for colon
        buffer.skip_space()
        colon_present = buffer.match(':', True)
        
        # Parse upper bound
        upper_bound = None
        buffer.skip_space()
        if in_range(buffer.read(), '0', '9'):
            upper_bound = parse_integer(buffer)
        
        # Check for a closing bracket
        buffer.skip_space()
        if not buffer.match('}', True):
            errors.add(section, 'expected \'}\'', buffer.position, buffer)
            return None
        
        # Make sure there was at least one value present
        if lower_bound is None and upper_bound is None:
            errors.add(section, 
                    'missing bounds values', 
                    bounds_position, 
                    buffer)

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
                errors.add(section, 
                        invalid_bounds[lower_bound], 
                        bounds_position, 
                        buffer)
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
                errors.add(section, 
                        invalid_bounds[lower_bound], 
                        bounds_position, 
                        buffer)
                return None

            return (lower_bound, -1)

        # Up to N instances
        # - term{:0} makes no sense
        # - term{:1} is the same as term?
        elif lower_bound is None and upper_bound is not None and colon_present:

            invalid_bounds = {
                0: 'zero-valued bound',
                1: 'improper bound (should be \'?\')'
            }

            if upper_bound in invalid_bounds:
                errors.add(section, 
                        invalid_bounds[upper_bound], 
                        bounds_position, 
                        buffer)
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
                errors.add(section, 
                        f'improper bounds (should be \'{{{lower_bound}}}\')', 
                        bounds_position, 
                        buffer)
                return None
            elif lower_bound > upper_bound:
                errors.add(section, 
                        f'invalid bounds ({lower_bound} > {upper_bound})', 
                        bounds_position, 
                        buffer)
                return None
            elif lower_bound == 0:
                errors.add(section, 
                        f'improper bounds (should be \'{{:{upper_bound}}}\')', 
                        bounds_position, 
                        buffer)
                return None
            
            return (lower_bound, upper_bound)
        
        # Failing the above, it's hard to say what went wrong; there's a chance
        # we parsed a _very_ short non-empty embedded expression, but that 
        # should be hard given we're only looking 8 characters ahead
        else:
            errors.add(section, f'invalid bounds', bounds_position, buffer)
            return None

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a term of any type

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        reference: Reference
            the parsed term
        '''

        domain = f'{Term.domain}:parse'

        binding = ''
        binding_position = copy(buffer.position)
        character = buffer.read()
        if (character == '_'
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')):
            binding = parse_identifier(buffer)

            buffer.skip_space()
            if not buffer.match(':', True):
                buffer.position = binding_position
                binding = ''

        buffer.skip_space()
        character = buffer.read()
        term = None
        if character == '(':
            term = grammar.terms.Sequence.parse(buffer, errors)
        elif character == '\'':
            term = grammar.terms.Constant.parse(buffer, errors)
        elif character == '[':
            term = grammar.terms.Range.parse(buffer, errors)
        elif (character == '_'
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')):
            term = grammar.terms.Reference.parse(buffer, errors)
        elif character == '`':
            term = grammar.terms.Set.parse(buffer, errors)

        else:
            errors.add(domain, 'expected a term', buffer.position, buffer)
            return None

        if not term:
            return None

        buffer.skip_space()
        term.bounds = Term.parse_bounds(buffer, errors)
        if not term.bounds:
            return None
        
        term.binding = binding
        return term

    @staticmethod
    def present(buffer: ParseBuffer) -> bool:
        ''' Checks for a term at the buffer's current position

        Arguments
        ---------
        buffer: ParseBuffer
            buffer in which to check for a term

        Returns
        -------
        term_present: bool
            true if there's a term at the buffer's current index which (could)
            be a term
        '''

        character = buffer.read()
        return (character == '\''
                or character == '['
                or in_range(character, 'a', 'z')
                or in_range(character, 'A', 'Z')
                or character == '('
                or character == '`')

    def link_references(self, 
            rules: dict, 
            buffer: ParseBuffer, 
            errors: ErrorBuffer) -> bool:
        
        ''' Links references (redundant override)

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
        buffer: ParseBuffer
            the buffer used for parsing
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        success: bool
            whether or not the linking succeeded
        '''

        return True
