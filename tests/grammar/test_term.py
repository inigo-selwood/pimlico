from perivale import Buffer

from grammar import Term


def test_term_parse_valid():

    # Simple term
    terms = [
        "(a | b)",
        "'constant'",
        "[az]",
        "(a b)",
        "reference",
        "`ab`",
    ]
    for text in terms:
        buffer = Buffer(text)
        assert Term.parse(buffer, [])
        assert buffer.finished()
    
    # Term with binding
    buffer = Buffer("name:term")
    term = Term.parse(buffer, [])
    assert term
    assert term.binding == "name"

    # Term with instance bounds
    terms = [
        "term+",
        "term*",
        "term?",
        "(term)+",
        
        "term<2>",
        "term<2:>",
        "term<:2>",
        "term<1:2>",
    ]
    for text in terms:
        buffer = Buffer(text)
        term =  Term.parse(buffer, [])
        assert term
        assert buffer.finished(), text
    
    # Term with predicate
    terms = {
        "!term": "not",
        "&term": "and",
    }
    for term, predicate in terms.items():
        buffer = Buffer(term)
        term = Term.parse(buffer, [])
        assert term
        assert term.predicate == predicate


def test_term_parse_invalid():

    # Invalid predicate + binding combination
    buffer = Buffer("name:!term")
    errors = []
    assert not Term.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "predicated terms cannot be bound"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 1

    # Bound < 0
    buffer = Buffer("term<-1:0>")
    errors = []
    assert not Term.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "invalid instance bound value '-1'"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 6

    # Trailing garbage
    buffer = Buffer("term<1:2:>")
    errors = []
    assert not Term.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected '>'"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 9

    # Illogical bound
    buffer = Buffer("term<1:1>")
    errors = []
    assert not Term.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "illogical instance bounds (1 >= 1)"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 5

    # Invalid bound format
    buffer = Buffer("term<1 1>")
    errors = []
    assert not Term.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "invalid instance bounds"
    assert errors[0].position.line == 1
    assert errors[0].position.column == 5

