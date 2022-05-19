import json

from controllers import build
from text import ErrorBuffer


def _walk_rule_tree(rule, stack: list = []) -> dict:
    result = {}

    stack.append(rule.name)
    for name, child in rule.imports.items():

        # Break recursion
        if name in stack:
            continue
    
        result[name] = _walk_rule_tree(child, stack)
    
    return result


def print_tree(node: str, 
        tree: dict, 
        rank: str = 'first', 
        leader: str = '', 
        tag: str = ''):
    
    if rank == 'first':
        print(node)
    else:
        print(f'{leader}{tag} {node}')
    
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
        print_tree(name, child, child_rank, new_leader, new_tag)

        index += 1


def tree(grammar_file: str, errors: ErrorBuffer):

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
    
    # print(json.dumps(tree, indent=4))
    print_tree('.', tree)


    return None