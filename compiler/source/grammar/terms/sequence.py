from copy import copy

from hashlib import sha256

from utilities import in_range
from text import Position, ParseBuffer, ErrorBuffer

from grammar import Term
from grammar.terms import Choice


class Sequence(Term):

    domain = 'grammar.terms.sequence'

    def __init__(self, values: list, position: Position):
        self.values = values
        self.position = position
        self.type = 'sequence'

        context = sha256()
        value_hashes = [value.hash for value in values]
        for hash in value_hashes:
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
        for index in range(value_count):
            result += self.values[index].__str__()
            
            if (index + 1) < value_count:
                result += ' '
        
        if enclosed:
            result += ')'
        
        return result

    @staticmethod
    def parse(buffer: ParseBuffer, errors: ErrorBuffer, root: bool = False):
        ''' Parses a sequence

        Where a sequence has the format: `term0 term1 ... termN`

        Sequences can also be enclosed in parentheses.

        Arguments
        ---------
        buffer: ParseBuffer
            buffer at a sequence term
        errors: ErrorBuffer
            buffer for reporting errors
        root: bool
            whether this sequence is the top-level of an expression

        Returns
        -------
        sequence: Sequence
            the parsed term
        '''

        character = buffer.read()
        assert Term.present(buffer)

        domain = f'{Sequence.domain}:parse'
        start_position = copy(buffer.position)

        enclosed = False
        if not root and buffer.match('(', True):
            enclosed = True

            buffer.skip_space()
            if buffer.match(')', True):
                errors.add(domain, 'empty sequence', start_position, buffer)
                return None

        values = []
        valid = True
        while True:

            term = Choice.parse(buffer, errors)

            if not term:
                return None
            elif values and term.hash == values[-1].hash:
                errors.add(domain, 
                        'redundant (instance hint)', 
                        term.position, 
                        buffer)
                valid = False
            else:
                values.append(term)

            buffer.skip_space()
            if (buffer.finished()
                    or buffer.match('\n')
                    or buffer.match('{')
                    or buffer.match(')')):
                break

        if not valid:
            return None
        elif enclosed and not buffer.match(')', True):
            errors.add(domain, 'expected \')\'', buffer.position, buffer)
            return None
        elif len(values) == 1:
            return values[0]

        return Sequence(values, start_position)

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
        for term in self.values:
            term_success = term.link_references(rules,
                    parent_rule, 
                    buffer, 
                    errors)
            success = success and term_success
        return success
