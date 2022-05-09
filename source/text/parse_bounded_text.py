from .parse_buffer import ParseBuffer
from .error_buffer import ErrorBuffer


def parse_bounded_text(buffer: ParseBuffer, 
        errors: ErrorBuffer, 
        start_sequence: str, 
        end_sequence: str = '',
        permit_newlines: bool = True) -> str:
    
    ''' Parses a length of text bounded by a start and end sequence

    For example, `{test}` is the string `test` bounded by the two sequences
    `{` and `}`

    If the start and end sequences are symmetric, omit the end sequence. ie:
    `### test ###`. Note: this means that the sequence can't occur within 
    itself.


    Arguments
    ---------
    buffer: ParseBuffer
        buffer to parse text from
    errors: ErrorBuffer
        buffer for error reporting
    start_sequence: str
        string marking the start of the section
    end_sequence: str = ''
        string marking the end of the section; omit if the start and end 
        sequences are the same
    permit_newlines: bool = True
        whether or not to report an error on encountering a newline
    
    Returns
    -------
    text: str
        the text found between the two sequences
    '''

    assert buffer.match(start_sequence, True)
    domain = 'text:parse_bounded_text'

    text = start_sequence

    # If there is no end sequence, it's considered symmetric; ie: the start 
    # sequence is also the end. In this case, there's no need for a stack,
    # because the first instance encountered marks the end
    if not end_sequence or start_sequence == end_sequence:

        while True:
            if buffer.finished():
                errors.add(domain, 
                        f'expected \'{start_sequence}\', got end-of-file', 
                        buffer.position)
                return None
            elif not permit_newlines and buffer.match('\n'):
                errors.add(domain, 'unexpected newline', buffer.position)
                return None
            elif buffer.match(start_sequence, True):
                text += start_sequence
                break
            else:
                text += buffer.read(True)
    
    # If the two sequences are distinct, we need to keep a stack of how many
    # of each are encountered, and only stop parsing when the matching 
    # end sequence is encountered
    else:

        stack = 1
        while True:
            if buffer.finished():
                errors.add(domain, 
                        f'expected \'{end_sequence}\', got end-of-file', 
                        buffer.position)
                return None
            elif not permit_newlines and buffer.match('\n'):
                errors.add(domain, 'unexpected newline', buffer.position)
                return None
            
            elif buffer.match(start_sequence):
                stack += 1
                text += start_sequence
                buffer.increment(len(start_sequence))
            elif buffer.match(end_sequence):
                stack -= 1
                text += end_sequence
                buffer.increment(len(end_sequence))
            
            else:
                text += buffer.read(True)
            
            if stack == 0:
                break

    return text