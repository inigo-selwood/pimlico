from __future__ import annotations

from copy import copy

from hashlib import sha256

import grammar
from grammar import terms
import text
from text import helpers
import tools

from ..term import Term


class Intrinsic(Term):

    def __init__(self, variant: str, position: text.Position):
        super(Intrinsic, self).__init__()

        self.variant = variant
        self.position = position
        self.type = 'intrinsic'

        context = sha256()
        context.update(variant.encode('utf-8'))
        self.hash = context.hexdigest()

    def __str__(self):

        result = ''
        if self.variant == '__space__':
            result += '_'
        elif self.variant == '__whitespace__':
            result += '__'
        else:
            result += self.variant
        
        return Term.decorate(self, result)
    
    @staticmethod
    def parse(buffer: text.Buffer, errors: tools.ErrorLog) -> grammar.Choice:
        
        if not buffer.match('_'):
            raise ValueError('expected an underscore')
        
        position = copy(buffer.position)

        variants = [
            '__character__',
            '__identifier__',
            '__integer__',
            '__newline__',
            '__number__',
        ]

        identifier = helpers.parse_identifier(buffer)
        if identifier == '_':
            return Intrinsic('__space__', position)
        elif identifier == '__':
            return Intrinsic('__whitespace__', position)
        elif identifier in variants:
            return Intrinsic(identifier, position)
        
        # If we can't match it, try to parse it as a reference
        buffer.position = position
        return terms.Reference.parse(buffer, errors)
    
    @Term.greedy_parser
    def match(self, buffer: text.Buffer):

        # Character
        if self.variant == '__character__':
            character = buffer.read()
            if not helpers.in_range(character, ' ', '~'):
                return (False, '')
            
            buffer.increment()
            return (True, character)
        
        # Identifier
        elif self.variant == '__identifier__':
            character = buffer.read()
            if (character != '_'
                    and not helpers.in_range(character, 'a', 'z')
                    and not helpers.in_range(character, 'A', 'Z')
                    and not helpers.in_range(character, '0', '9')):
                return (False, '')
            
            value = helpers.parse_identifier(buffer)
            return (True, value)

        # Integer
        elif self.variant == '__integer__':
            character = buffer.read()
            if not helpers.in_range(character, '0', '9'):
                return (False, '')
            
            value = helpers.parse_integer(buffer)
            return (True, value)

        # Newline
        elif self.variant == '__newline__':
            result = buffer.match('\n', consume=True)
            if result:
                return (True, '\n')
            return (False, '')
        
        # Floating point IEEE754
        elif self.variant == '__number__':
            text = ''
            
            # Parse integer portion
            # Check for minus symbol
            if buffer.match('-', consume=True):
                text += '-'
            
            # Parse integer itself
            if not helpers.in_range(buffer.read(), '0', '9'):
                return (False, '')
            text += helpers.parse_integer(buffer)

            # Check for decimal value
            if buffer.match('.', consume=True):
                text += '.'

                if not helpers.in_range(buffer.read(), '0', '9'):
                    return (False, '')
                text += helpers.parse_integer(buffer)

            # Look for exponent
            for exponent in ['e', 'E']:
                if buffer.match(exponent, consume=True):
                    text += exponent
                    exponent_present = True
                    break
            
            # Didn't know Python had this syntactic sugar, but this will match 
            # if the loop doesn't break
            else:
                return (True, text)
            
            # Parse the exponent's value
            # Check for a minus symbol
            if buffer.match('-', consume=True):
                text += '-'
            
            # Parse exponent value
            if not helpers.in_range(buffer.read(), '0', '9'):
                return (False, '')
            text += helpers.parse_integer(buffer)

            return (True, text)
        
        elif self.variant == '__space__':
            character = buffer.read()

            space_characters = ' \t\v\r'
            if character in space_characters:
                buffer.increment()
                return (True, character)
            return (False, '')
        
        elif self.variant == '__whitespace__':
            character = buffer.read()

            space_characters = ' \t\v\n\r'
            if character in space_characters:
                buffer.increment()
                return (True, character)
            return (False, '')
