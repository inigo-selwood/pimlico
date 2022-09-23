import sys

from amersham import Parser

from perivale import Buffer

from grammar import Source


parser = Parser("pimlico", "a PEG parser generator for Python")


@parser.command(source={"description": "file to build"})
def build(source: str):

    text = ""
    with open(source, 'r') as file:
        text = file.read()

    buffer = Buffer(text)
    errors = []

    source = Source.parse(buffer, errors)
    if not source:
        for error in errors:
            print(error)

if __name__ == "__main__":
    parser.run(sys.argv[1:])