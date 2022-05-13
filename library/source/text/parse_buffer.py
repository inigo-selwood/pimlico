from ctypes import *


_library = cdll.LoadLibrary('./library.so')


class Position(Structure):

    _fields_ = [
        ('index', c_uint32),
        ('line', c_int32),
        ('column', c_int16)
    ]

    def __new__(ctr):
        callable = _library.positionCreate
        callable.restype = Position
        return callable()
    
    def __str__(self):
        callable = _library.positionPrint
        callable.argtypes = [POINTER(Position), c_char_p, c_size_t]
        callable.restype = c_uint8

        buffer_size = 16
        text = create_string_buffer(buffer_size)
        assert callable(byref(self), text, buffer_size)

        return str(text.value, 'utf-8')


class ParseBuffer:

    class Object(Structure):

        _fields_ = [
            ('text', c_char_p),
            ('length', c_uint32),
            ('position', Position),

            ('lineCount', c_uint32),
            ('lineIndices', POINTER(c_uint32)),
            ('lineIndentations', POINTER(c_uint8))
        ]
    
    def __init__(self, text: str):
        ''' Constructs the buffer object

        Arguments
        ---------
        text: str
            text to fill the buffer with
        '''
        
        self.object = None
        assert text

        callable = _library.parseBufferCreate
        callable.argtypes = [c_char_p]
        callable.restype = POINTER(ParseBuffer.Object)
        self.object = callable(text.encode())
        
    def __del__(self):
        ''' Frees the memory held by the buffer
        '''

        if self.object:
            callable = _library.parseBufferDestroy
            callable.argtypes = [POINTER(ParseBuffer.Object)]
            callable.restype = c_uint8
            assert callable(self.object)
    
    def get_character(self) -> str:
        ''' Gets a single character from the buffer

        Returns
        -------
        character: str
            the character found, or None if the end of the buffer has already
            been reached
        '''

        callable = _library.parseBufferGetCharacter
        callable.argtypes = [POINTER(ParseBuffer.Object), c_char_p]
        callable.restype = c_uint8

        character = create_string_buffer(1)
        assert callable(self.object, character)
        return str(character.value, 'utf-8')
    
    def finished(self) -> bool:
        ''' Checks whether the buffer is finished

        Returns
        -------
        finished: bool
            True if the end of the buffer has been reached
        '''

        callable = _library.parseBufferFinished
        callable.argtypes = [POINTER(ParseBuffer.Object), POINTER(c_uint8)]
        callable.restype = c_uint8

        finished = c_uint8()
        assert callable(self.object, byref(finished))
        return bool(finished)
    
    def increment(self, steps: int = 1):
        ''' Increments the position in the buffer, updating the line and column

        Arguments
        ---------
        steps: int 
            the number of steps to increment the position by
        '''

        callable = _library.parseBufferIncrement
        callable.argtypes = [POINTER(ParseBuffer.Object), c_uint16]
        callable.restype = c_uint8
        assert callable(self.object, steps)

    def read(self, consume: bool = False) -> str:
        ''' Read the next character from the buffer, optionally incrementing

        Arguments
        ---------
        consume: bool
            if true, increments past the character just read

        Returns
        -------
        character: str
            the next character in the buffer
        '''

        callable = _library.parseBufferRead
        callable.argtypes = [POINTER(ParseBuffer.Object), c_char_p, c_uint8]
        callable.restype = c_uint8

        next_character = create_string_buffer(1)
        assert callable(self.object, next_character, consume)

        return str(next_character.value, 'utf-8')

    def line_indentation(self, line_number: int = 0) -> int:
        ''' Gets the indentation of the current line (default), or a given one

        Arguments
        ---------
        line_number: int
            the index of the line to get the indentation of (optional)
        
        Returns
        -------
        indentation: int
            the indentation level in steps, where a tab is 4, and a space is 1
        '''

        callable = _library.parseBufferLineIndentation
        callable.argtypes = [
            POINTER(ParseBuffer.Object), 
            POINTER(c_uint8), 
            c_uint32
        ]
        callable.restype = c_uint8

        indentation = c_uint8()
        assert callable(self.object, byref(indentation), line_number)
        return indentation.value
    
    def line_text(self, line_number: int = 0) -> str:
        ''' Gets the text of the current line (default), or a given one

        Arguments
        ---------
        line_number: int
            the index of the line to get the text from (optional)
        
        Returns
        -------
        text: str
            the text of the current line, from newline to newline
        '''

        callable = _library.parseBufferLineText
        callable.argtypes = [
            POINTER(ParseBuffer.Object), 
            c_char_p, 
            c_size_t,
            c_uint32
        ]
        callable.restype = c_uint8

        buffer_size = 128
        text = create_string_buffer(buffer_size)
        assert callable(self.object, text, buffer_size, line_number)

        return str(text.value, 'utf-8')
    
    def match(self, text: str, consume: bool = False) -> bool:
        ''' Checks for a string match, optionally consuming it

        Arguments
        ---------
        text: str
            the text to try and match
        consume: bool
            whether to increment past the value (if matched), optional
        
        Returns
        -------
        match: bool
            whether the string matched
        '''

        callable = _library.parseBufferMatch
        callable.argtypes = [
            POINTER(ParseBuffer.Object),
            c_char_p,
            POINTER(c_uint8),
            c_uint8
        ]
        callable.restype = c_uint8

        match = c_uint8()
        assert callable(self.object, text.encode(), byref(match), consume)

        return bool(match)
    
    def seek(self, text: str, consume: bool = False, limit: int = -1) -> bool:
        ''' Looks ahead a limited number of steps to try and match a string

        Note: Whitespace characters don't count towards the limit

        Arguments
        ---------
        text: str
            the text to try and match
        consume: bool
            if true, increments past any matched value
        limit: int
            the maximum number of characters to look ahead (not inclusive), 
            before abandoning the search
        '''

        callable = _library.parseBufferSeek
        callable.argtypes = [
            POINTER(ParseBuffer.Object), 
            c_char_p, 
            POINTER(c_uint8), 
            c_uint8, 
            c_uint8
        ]
        callable.restype = c_uint8

        match = c_uint8()
        assert callable(self.object, 
                text.encode(), 
                byref(match), 
                consume, 
                limit)
            
        return bool(match)
    
    def skip_line(self):
        ''' Skips to the next newline (or EOF)
        '''

        callable = _library.parseBufferSkipLine
        callable.argtypes = [POINTER(ParseBuffer.Object)]
        callable.restype = c_uint8

        assert callable(self.object)


    def skip_space(self, include_newlines: bool = False):
        ''' Skips whitespace characters, optionally including newlines

        Arguments
        ---------
        include_newlines: bool
            if true, skips newline characters as well
        '''

        callable = _library.parseBufferSkipSpace
        callable.argtypes = [POINTER(ParseBuffer.Object), c_uint8]
        callable.restype = c_uint8

        assert callable(self.object, include_newlines)
