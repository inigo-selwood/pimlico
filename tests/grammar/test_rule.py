from perivale import Buffer

from grammar import Rule


def test_rule_parse_valid():

    # Single production
    text = """ruleName
    := reference {
        return __text__
    }"""
    buffer = Buffer(text)
    rule = Rule.parse(buffer, [])
    assert rule
    assert rule.name == "ruleName"
    assert buffer.finished()

    # Multiple productions
    text = """ruleName
    := reference {
        return __text__
    }
    := otherReference {
        return f"other-{__text__}"
    }"""
    buffer = Buffer(text)
    rule = Rule.parse(buffer, [])
    assert rule
    assert buffer.finished()

    # With type hint
    text = """ruleName -> package.RuleType
    := reference"""
    buffer = Buffer(text)
    rule = Rule.parse(buffer, [])
    assert rule
    assert rule.type_hint == "package.RuleType"
    assert buffer.finished()

def test_rule_parse_invalid():

    # Missing type hint
    buffer = Buffer("ruleName ->\n")
    errors = []
    assert not Rule.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected a type hint"
    assert errors[0].position.line == 1
    assert errors[0].position.column == -1

    # No assignment
    buffer = Buffer("ruleName\n")
    errors = []
    assert not Rule.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected a rule assignment (':=')"
    assert errors[0].position.line == 1
    assert errors[0].position.column == -1

    # Missing statement
    text = """ruleName
    := {}"""
    buffer = Buffer(text)
    errors = []
    assert not Rule.parse(buffer, errors)
    assert len(errors) == 1
    assert errors[0].message == "expected production terms"
    assert errors[0].position.line == 2
    assert errors[0].position.column == 8