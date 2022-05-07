from copy import copy

from hashlib import sha256

from text import Position, ParseBuffer, ErrorBuffer

from grammar import Term


class Choice:

    domain = 'grammar.terms.choice'

    def __init__(self, values: dict, position: Position):
        self.values = values
        self.position = position
        self.type = 'choice'
        self.bounds = (1, 1)

        context = sha256()
        for hash in sorted(values.keys()):
            context.update(hash.encode('utf-8'))
        self.hash = context.hexdigest()

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
                errors.add(domain, 'duplicate option', term.position)
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
                    errors.add(domain, 'empty option', pipe_position)
                    valid = False
                else:
                    break

                pipe_position = new_pipe_position

            if buffer.finished():
                errors.add(domain, 'unexpected end-of-file', buffer.position)
                return None
            elif buffer.match('\n'):
                errors.add(domain, 'unexpected end-of-line', buffer.position)
                return None

        if not valid:
            return None
        elif len(options) == 1:
            return list(options.values())[0]

        return Choice(options, start_position)
