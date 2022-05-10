from grammar import Program

from .helpers import run_invalid_parse_test, run_valid_parse_test


def test_simple():
    text = ('rule0 := \'term0\''
            '\nrule1 := \'term1\'')
    run_valid_parse_test(text, Program.parse)


def test_inclusion():
    run_valid_parse_test('.include(string)', Program.parse)


def test_multi_line():
    text = ('rule0 :='
            '\n    - \'term0\' {}'
            '\n    - \'term1\' {}'
            '\n'
            '\nrule1 := \'term2\' {}')
    run_valid_parse_test(text, Program.parse)   


def test_multiple_references():
    text = ('\nrule1 := \'a\' | \'b\' # Choice'
            '\nrule2 := \'a\' # Constant'
            '\nrule3 := rule1 # Reference'
            '\nrule4 := \'a\' \'b\' # Sequence'
            '\nrule5 := `az` # Set')
    run_valid_parse_test(text, Program.parse)   


def test_empty_text():
    run_invalid_parse_test('', Program.parse, [('empty text', (-1, -1))])


def test_unexpected_indentation_increase():
    errors = [('unexpected indentation increase', (1, 5))]
    run_invalid_parse_test('    rule := \'term\'', Program.parse, errors)


def test_expected_rule():
    run_invalid_parse_test('{}', Program.parse, [('expected a rule', (1, 1))])


def test_duplicate_rule():
    text = ('rule := \'term\''
            '\nrule := \'otherTerm\'')
    errors = [
        ('duplicate rule \'rule\'', (2, 1)),
        ('first declared here', (1, 1))
    ]
    run_invalid_parse_test(text, Program.parse, errors)


def test_trailing_garbage():
    run_invalid_parse_test('rule := \'term\' {} {}', 
            Program.parse, 
            [('expected a newline', (1, 19))])


def test_undefined_reference():
    errors = [('undefined reference to rule \'reference\'', (1, 9))]
    run_invalid_parse_test('rule := reference', Program.parse, errors) 