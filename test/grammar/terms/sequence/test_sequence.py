from glob import glob

from text import ParseBuffer, ErrorBuffer

from grammar.terms import Sequence


def test_root():
    files = glob('grammar/terms/sequence/examples/*.txt')
    grammars = []
    for file in files:

        with open(file, 'r') as file:
            grammars.append(file.read())

    for grammar in grammars:
        buffer = ParseBuffer(grammar)
        errors = ErrorBuffer()
        term = Sequence.parse(buffer, errors, True), grammar

        print(errors.serialize(buffer))
        assert term
        assert buffer.finished(), grammar


def test_reduction():
    term = Sequence.parse(ParseBuffer('\'a\''), ErrorBuffer())
    assert term.type == 'constant'