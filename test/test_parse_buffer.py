from parse_buffer import ParseBuffer


class TestParseBuffer:

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
        assert not buffer.read()
        assert buffer.finished()

    def test_peek(self):
        buffer = ParseBuffer(' ')
        assert buffer.peek() == ' '
        assert not buffer.finished()

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

    def test_skip_space(self):
        buffer = ParseBuffer(' \t\r\v')
        buffer.skip_space()
        assert buffer.finished()

    def test_skip_line(self):
        buffer = ParseBuffer('\n\n')
        buffer.skip_line()
        assert buffer.position.line == 2

        buffer = ParseBuffer('\n')
        buffer.skip_line()
        assert buffer.finished()
