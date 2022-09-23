from __future__ import annotations

from perivale import Buffer

from grammar import Rule


class Source:

    def __init__(self, rules: dict):
        self.rules = rules
    
    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Source:

        rules = {}
        while True:

            buffer.skip_space(include_newlines=True)
            if buffer.finished():
                break
            
            rule = Rule.parse(buffer, errors)
            if not rule:
                return None
            
            # Check rule not already defined
            if rule.name in rules:
                old_rule = rules[rule.name]
                message = "duplicate rule definition"
                errors.append(buffer.error(message, rule.position))
                
                message = "first defined here"
                errors.append(buffer.error(message, old_rule.position))
                return None
        
            rules[rule.name] = rule
        
        success = True
        for rule in rules.values():
            success = success and rule.bind_references(buffer, rules, errors)
        if not success:
            return None
        
        if "__root__" not in rules:
            errors.append(buffer.error("'__root__' undefined"))
            return None
        
        return Source(rules)