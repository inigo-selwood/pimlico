from ctypes import *


_library = cdll.LoadLibrary('./libparse_buffer.so')


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
        buffer = create_string_buffer(buffer_size)
        callable(byref(self), buffer, buffer_size)

        return str(buffer.value, 'utf-8')


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
    
    def __init__(self, text):
        self.object = None
        assert text

        callable = _library.parseBufferCreate
        callable.argtypes = [c_char_p]
        callable.restype = POINTER(ParseBuffer.Object)
        self.object = callable(text.encode())
        
    def __del__(self):

        if self.object:
            callable = _library.parseBufferDestroy
            callable.argtypes = [POINTER(ParseBuffer.Object)]
            callable.restype = c_uint8
            assert callable(self.object)
    
    def get_character(self) -> str:
        callable = _library.parseBufferGetCharacter
        callable.argtypes = [POINTER(ParseBuffer.Object), c_char_p]
        callable.restype = c_uint8

        character = create_string_buffer(1)
        assert callable(self.object, character)
        return str(character.value, 'utf-8')
    
    def finished(self) -> bool:
        callable = _library.parseBufferFinished
        callable.argtypes = [POINTER(ParseBuffer.Object), POINTER(c_uint8)]
        callable.restype = c_uint8

        finished = c_uint8()
        assert callable(self.object, byref(finished))
        return bool(finished)
    
    def increment(self, steps: int = 1):
        callable = _library.parseBufferIncrement
        callable.argtypes = [POINTER(ParseBuffer.Object), c_uint16]
        callable.restype = c_uint8
        assert callable(self.object, steps)

    def read(self, consume: bool = False) -> str:
        callable = _library.parseBufferRead
        callable.argtypes = [POINTER(ParseBuffer.Object), c_char_p, c_uint8]
        callable.restype = c_uint8

        next_character = create_string_buffer(1)
        assert callable(self.object, next_character, consume)

        return str(next_character.value, 'utf-8')

    def line_indentation(self, line_number: int = 0) -> int:
        callable = _library.parseBufferLineIndentation
        callable.argtypes = [
            POINTER(ParseBuffer.Object), 
            POINTER(c_uint8), 
            c_uint32
        ]
        callable.restype = c_uint8

        indentation = c_uint8()
        assert callable(self.object, byref(indentation), 0)
        return indentation.value


if __name__ == '__main__':
    buffer = ParseBuffer('    hello world')
    print(buffer.object.contents.lineIndentations[0])
    print(buffer.line_indentation())
    del buffer