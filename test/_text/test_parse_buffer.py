from text import ParseBuffer


class TestParseBuffer:

    def test_empty_position(self):
        buffer = ParseBuffer('')
        assert (buffer.position.line, buffer.position.column) == (-1, -1)

    def test_position(self):
        buffer = ParseBuffer(' ')
        assert (buffer.position.line, buffer.position.column) == (1, 1)
    
    def test_newline_position(self):
        buffer = ParseBuffer('\n')
        assert (buffer.position.line, buffer.position.column) == (1, -1)

    def test_finished(self):
        buffer = ParseBuffer('')
        assert buffer.finished()

        buffer = ParseBuffer(' ')
        assert not buffer.finished()

    def test_increment(self):
        buffer = ParseBuffer(' ')
        buffer.increment()
        assert buffer.finished()

        buffer = ParseBuffer('  ')
        buffer.increment(2)
        assert buffer.finished()

        buffer = ParseBuffer(' ')
        buffer.increment(2)
        assert buffer.finished()

    def test_read(self):
        buffer = ParseBuffer(' ')
        assert buffer.read() == ' '
        assert not buffer.finished()

        buffer = ParseBuffer(' ')
        assert buffer.read(True) == ' '
        assert buffer.finished()

    def test_line_indentation(self):
        buffer = ParseBuffer('    \n')
        assert buffer.line_indentation(1) == 4
        assert buffer.line_indentation() == 4

    def test_line_text(self):
        buffer = ParseBuffer('\ntest')
        assert buffer.line_text(1) == ''
        assert buffer.line_text(2) == 'test'

    def test_match(self):
        buffer = ParseBuffer('test')
        assert buffer.match('test')
        assert not buffer.finished()

        assert buffer.match('test', True)
        assert buffer.finished()
    
    def test_seek(self):

        # Seek, and no range limit
        buffer = ParseBuffer(' test')
        assert buffer.seek('test')
        assert (buffer.position.line, buffer.position.column) == (1, 1)
        
        # Seek, consuming
        assert buffer.seek('test', True)
        assert buffer.finished()

        # Seek (but out of range)
        buffer = ParseBuffer('__test')
        assert not buffer.seek('test', limit=1)
        assert (buffer.position.line, buffer.position.column) == (1, 1)

        # Seek (out of range, but whitespace)
        buffer = ParseBuffer('  test')
        assert buffer.seek('test', limit=1)
        assert (buffer.position.line, buffer.position.column) == (1, 1)

    def test_skip_space(self):
        buffer = ParseBuffer(' \t\r\v')
        buffer.skip_space()
        assert buffer.finished()

        buffer = ParseBuffer('\n ')
        buffer.skip_space(True)
        assert buffer.finished()

    def test_skip_line(self):

        for text in ['\n', ' \n']:
            buffer = ParseBuffer(text)
            buffer.skip_line()
            assert buffer.position.line == 1
            assert buffer.position.column == -1