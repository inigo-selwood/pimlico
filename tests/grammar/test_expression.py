from perivale import Buffer

from grammar import Expression


def test_expression_parse_valid():

    # Single-line expression
    text = "{ return None }"
    buffer = Buffer(text)
    expression = Expression.parse(buffer, [])
    assert expression
    assert expression.text == "return None"
    assert buffer.finished

    # Multi-line expression
    text = """{
        result = [head]
        result.extend(tail)
        return result
    }"""
    buffer = Buffer(text)
    expression = Expression.parse(buffer, [])
    assert expression
    assert buffer.finished()

    # Expression containing map
    text = """{
        map = {
            'object': {},
            'other-object': {},
        }
        return map
    }"""
    buffer = Buffer(text)
    expression = Expression.parse(buffer, [])
    assert expression
    assert buffer.finished()


def test_expression_parse_invalid():

    # End-of-file
    buffer = Buffer("{")
    errors = []
    assert not Expression.parse(buffer, errors)
    assert len(errors) == 1

    message = "unexpected end-of-file before '}' in expression"
    assert errors[0].message == message
    assert errors[0].position.line == 1
    assert errors[0].position.column == -1

    # Under-indentation (mid-expression)
    text = """{
}"""
    buffer = Buffer(text)
    errors = []
    assert not Expression.parse(buffer, errors)
    assert len(errors) == 1

    message = "line under-indented in expression"
    assert errors[0].message == message
    assert errors[0].position.line == 2
    assert errors[0].position.column == 1

    # Under-indentation (end-of-expression)
    text = """{
return None
}"""
    buffer = Buffer(text)
    errors = []
    assert not Expression.parse(buffer, errors)
    assert len(errors) == 1

    message = "line under-indented in expression"
    assert errors[0].message == message
    assert errors[0].position.line == 2
    assert errors[0].position.column == 1