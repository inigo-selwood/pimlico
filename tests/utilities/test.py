from perivale import Buffer, Excerpt, Position


def run_parser(parser: callable, text: str) -> any:
    buffer = Buffer(text)
    errors = []
    result = parser(buffer, errors)

    for error in errors:
        raise Exception(error.__str__())
    
    assert buffer.finished()
    assert result

    return result


def run_parser_fail(parser: callable, text: str) -> list:
    buffer = Buffer(text)
    errors = []
    result = parser(buffer, errors)
    assert not result

    return errors


def error_present(errors: list, excerpts: list):

    excerpt_count = len(excerpts)
    for error in errors:

        if len(error.excerpts) != excerpt_count:
            continue
    
        for index in range(excerpt_count):
            message, position = excerpts[index]
            line, column = position

            excerpt_message, excerpt = error.excerpts[index]
            if (excerpt_message == message 
                    and excerpt.position.line == line 
                    and excerpt.position.column == column):
                return True

    return False