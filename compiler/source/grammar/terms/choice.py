from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer

from grammar import Term


class Choice(Term):

    domain = 'grammar.terms.choice'

    def __init__(self, values: dict, position: Position):
        self.values = values
        self.position = position
        self.type = 'choice'

        context = sha256()
        for hash in sorted(values.keys()):
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()
    
    def __str__(self):
        result = ''

        enclosed = False
        if self.binding:
            result = f'{self.binding}: '
            enclosed = True
        
        if enclosed:
            result += '('

        value_count = len(self.values)
        index = 0
        for value in self.values.values():

            # If the term is a sequence within a choice, it needs brackets. 
            # However, sequences with bindings print their own brackets, in 
            # which case we don't have to add them a second time
            if value.type == 'sequence' and not value.binding:
                result += f'({value.__str__()})'
            else:
                result += value.__str__()
            
            if (index + 1) < value_count:
                result += ' | '
            
            index += 1
        
        if enclosed:
            result += ')'
        
        return result

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer):
        ''' Parses a choice

        Where a choice has the format: `term0 | term1 | ... | termN`

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a [potential choice] term
        errors: ErrorBuffer
            buffer for reporting errors

        Returns
        -------
        choice: Choice
            the parsed term
        '''

        start_position = copy(buffer.position)

        domain = f'{Choice.domain}:parse'

        options = {}
        valid = True
        while True:
            term = Term.parse(buffer, errors)
            if not term:
                return None
            elif term.hash in options:
                errors.add(domain, 'duplicate option', term.position, buffer)
                valid = False
            else:
                options[term.hash] = term

            # Check for a pipe character, stop parsing if we don't find one
            buffer.skip_space()
            pipe_position = copy(buffer.position)
            if not buffer.match('|', True):
                break

            # Look ahead to see if there are more pipe characters; report each
            # one as an empty option  
            while True:
                buffer.skip_space()
                new_pipe_position = copy(buffer.position)
                if buffer.match('|', True):
                    errors.add(domain, 'empty option', pipe_position, buffer)
                    valid = False
                else:
                    break

                pipe_position = new_pipe_position

            if buffer.finished():
                errors.add(domain, 
                        'unexpected end-of-file', 
                        buffer.position, 
                        buffer)
                return None
            elif buffer.match('\n'):
                errors.add(domain, 
                        'unexpected newline', 
                        buffer.position, 
                        buffer)
                return None

        if not valid:
            return None
        elif len(options) == 1:
            return list(options.values())[0]

        return Choice(options, start_position)

    def link_references(self, 
            rules: dict, 
            parent_rule,
            buffer: ParseBuffer, 
            errors: ErrorBuffer) -> bool:
    
        ''' Links rules to this term's children

        Arguments
        ---------
        rules: dict
            the full list of rules in the program
        parent_rule: Rule
            the rule in which this term is defined
        buffer: ParseBuffer
            the buffer used for parsing
        errors: ErrorBuffer
            buffer for reporting errors
        
        Returns
        -------
        success: bool
            whether or not the linking succeeded
        '''

        success = True
        for term in self.values.values():
            term_success = term.link_references(rules, 
                    parent_rule, 
                    buffer, 
                    errors)
            success = success and term_success
        return success