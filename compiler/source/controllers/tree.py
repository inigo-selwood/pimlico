from controllers import build
from text import ErrorBuffer


def _walk_rule_tree(rule, stack: list = []) -> dict:
    ''' Walks a rule and its dependencies to construct a tree

    Arguments
    ---------
    rule: Rule
        the rule to graph
    stack: list
        the previously walked nodes, to prevent infinite recursion
    
    Returns
    -------
    tree: dict
        the tree of rules and their dependencies
    '''
    
    result = {}

    stack.append(rule.name)
    for name, child in rule.imports.items():

        # Break recursion
        if name in stack:
            result[name] = {}
            continue
    
        result[name] = _walk_rule_tree(child, stack)
    
    return result


def _print_tree(node: str, 
        tree: dict, 
        rank: str = 'first', 
        leader: str = '', 
        tag: str = '') -> str:
    
    ''' Formats a node and its children in the style of the `tree` command

    Arguments
    ---------
    node: str
        the name of the current node
    tree: dict
        this node's children
    rank: str
        the rank ('first', 'last', or '')
    leader: str
        the effective indent that precedes the current node
    tag: str
        the little lead-on string
    
    Returns
    -------
    result: str
        the string representation of the tree
    '''
    
    result = ''
    if rank == 'first':
        result += f'\n{node}'
    else:
        result += f'\n{leader}{tag} {node}'
    
    child_count = len(tree)
    index = 0
    for name, child in tree.items():
        child_last = (index + 1) == child_count
        child_rank = 'last' if child_last else ''
        
        new_tag = '╰─' if child_last else '├─'

        extension = ''
        if rank == 'last':
            extension = '   '
        elif rank != 'first':
            extension = '│  '
        
        new_leader = f'{leader}{extension}'
        result += _print_tree(name, child, child_rank, new_leader, new_tag)

        index += 1
    
    return result


def tree(grammar_file: str, errors: ErrorBuffer) -> str:
    ''' Constructs and prints a dependency tree for all rules in a grammar

    Arguments
    ---------
    grammar_file: str
        the name of the grammar file to graph
    errors: ErrorBuffer
        for reporting errors
    
    Returns
    -------
    result: str
        the printed tree, formatted as a string -- or None if an error occured
    '''

    program = build(grammar_file, errors)
    if not program:
        return None

    tree = {}
    for name, rule in program.rules.items():

        # Rules which have exports aren't root-level and can be ignored; 
        # they'll be picked up when the rule's tree gets walked
        if rule.exports:
            continue
            
        tree[name] = _walk_rule_tree(rule)
    
    return _print_tree('.', tree)
