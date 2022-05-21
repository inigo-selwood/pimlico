from copy import copy

import text
import tools
from text import helpers

def _parse_indentation(buffer: text.ParseBuffer, 
        errors: tools.ErrorLog) -> list:
    
    result = []
    while True:

        if (buffer.match('\t', consume=True) 
                or buffer.match('    ', consume=True)):
            result.append('indent')
        
        elif buffer.match(' '):
            buffer.skip_space()
            errors.add(__name__, 'invalid indentation', buffer.excerpt())
            return None
        
        else:
            break

    return result


def _parse_tokens(buffer: text.ParseBuffer, errors: tools.ErrorLog) -> list:

    # Newlines
    if buffer.match('\n', consume=True):
        result = ['newline']
        result.extend(_parse_indentation(buffer, errors))
        return result
        
    # Comments
    elif buffer.match('#', consume=True):
        buffer.skip_space()

        text = ''
        while True:
            if buffer.finished() or buffer.match('\n'):
                break
            else:
                text += buffer.read(consume=True)
        
        return [('comment', text)]
    
    # Expressions
    elif buffer.match('{{'):
        text = helpers.parse_expression(buffer, 
                ('{{', '}}'), 
                errors, 
                permit_newlines=True)
        if text is None:
            return None
        
        text = text[2:-2].strip()
        return [('expression', text)]
    
    # Constants
    elif buffer.match('\''):
        start_position = copy(buffer.position)

        text = helpers.parse_expression(buffer, 
                ('\'', '\''), 
                errors, 
                escape_codes=True)
        if text is None:
            return None
        
        text = text[1:-1]
        if not text:
            errors.add(__name__, 
                    'empty constant', 
                    buffer.excerpt(start_position))
            return None

        return [('constant', text)]
    
    # Sets
    elif buffer.match('`'):
        start_position = copy(buffer.position)
        
        text = helpers.parse_expression(buffer, 
                ('`', '`'), 
                errors, 
                escape_codes=True)
        if text is None:
            return None
        
        text = text[1:-1]
        if len(text) < 2:
            errors.add(__name__, 
                    'too few characters in set', 
                    buffer.excerpt(start_position))
            return None
        
        return [('set', text)]
    
    # Identifiers
    character = buffer.read()
    if (character == '_' 
            or helpers.in_range(character, 'a', 'z')
            or helpers.in_range(character, 'A', 'Z')):
        return [('identifier', helpers.parse_identifier(buffer))]
    
    # Numbers
    elif helpers.in_range(character, '0', '9'):
        return [('integer', helpers.parse_integer(buffer))]

    # Constants
    constants = [
        ':=', '-',
        '|', # Choices
        '(', ')', # Sequences
        '[', ']', # Ranges
        '{', ':', '}', # Specific bounds
        '+', '*', '?', # Shorthand bounds
        '!', '&' # Predicates
    ]

    for constant in constants:
        if buffer.match(constant, consume=True):
            return [constant]
    
    errors.add(__name__, 'expected a token', buffer.excerpt())
    return None        


def tokenize(grammar: str, errors: tools.ErrorLog) -> list:

    buffer = text.ParseBuffer(grammar)

    result = _parse_indentation(buffer, errors)
    
    while True:

        buffer.skip_space()
        if buffer.finished():
            break
    
        tokens = _parse_tokens(buffer, errors)
        if tokens is None:
            return None
        
        result.extend(tokens)
    
    return result