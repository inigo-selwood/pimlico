from perivale import Buffer

from grammar import Source


def test_parse_source_valid():

    text = """
__root__
    := ruleName
    
ruleName
    := 'constant'
    """
    buffer = Buffer(text)
    source = Source.parse(buffer, [])
    assert source
    assert "__root__" in source.rules
    assert "ruleName" in source.rules
    assert buffer.finished()


def test_parse_source_invalid():

    # Duplicate rule definition
    text = """
__root__
    := 'a constant'
    
__root__
    := 'another constant'
    """
    buffer = Buffer(text)
    errors = []
    assert not Source.parse(buffer, errors)
    assert len(errors) == 2

    assert errors[0].message == "duplicate rule definition"
    assert errors[0].position.line == 5
    assert errors[0].position.column == 1

    assert errors[1].message == "first defined here"
    assert errors[1].position.line == 2
    assert errors[1].position.column == 1

    # Root undefined
    text = """
notRoot
    := 'constant'"""
    buffer = Buffer(text)
    errors = []
    assert not Source.parse(buffer, errors)

    assert len(errors) == 1
    assert errors[0].message == "'__root__' undefined"
    assert errors[0].position.line == 3
    assert errors[0].position.column == -1

    # Undefined reference
    text = """
__root__
    := reference"""
    buffer = Buffer(text)
    errors = []
    assert not Source.parse(buffer, errors)

    assert len(errors) == 1
    assert errors[0].message == "undefined reference"
    assert errors[0].position.line == 3
    assert errors[0].position.column == 8
    