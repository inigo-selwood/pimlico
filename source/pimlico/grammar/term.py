from perivale import Buffer, Set, ParseException

import grammar


class Term:

    def __init__(self):
        self.predicate = None
        self.position = None
        self.binding = None
        self.instances = (1, 1)
    
    @staticmethod
    def present(buffer: Buffer) -> bool:
        character = buffer.read()
        return (character in "!?'[(`"
                or character == "_"
                or (character >= "a" and character <= "z")
                or (character >= "A" and character <= "Z"))
    
    @staticmethod
    def parse(buffer: Buffer, errors: list):
        position = buffer.copy_position()

        binding = None
        try:
            binding = buffer.parse_set(Set.SNAKE, consume=True)
            buffer.skip_space()
            if not buffer.match(":", consume=True):
                buffer.position = position
                position = buffer.copy_position()
        except ParseException:
            pass

        predicate = None
        if buffer.match("!", consume=True):
            predicate = "not"
        elif buffer.match("&", consume=True):
            predicate = "and"
    
        if predicate and binding:
            message = "predicated terms cannot be bound"
            errors.append(buffer.error(message, position))
            return None

        term = None
        buffer.skip_space()
        character = buffer.read()
        if character == "'":
            term = grammar.terms.Constant.parse(buffer, errors)
        elif character == "[":
            term = grammar.terms.Range.parse(buffer, errors)
        elif (character == "_" 
                or (character >= "0" and character <= "9")
                or (character >= "a" and character <= "z")
                or (character >= "A" and character <= "Z")):
            term = grammar.terms.Reference.parse(buffer, errors)
        elif character == "(":
            term = grammar.terms.Sequence.parse(buffer, errors)
        elif character == "`":
            term = grammar.terms.Set.parse(buffer, errors)
        else:
            errors.append(buffer.error("expected a term"))
            return None
        
        if term is None:
            return None
        
        def parse_instances() -> tuple:

            def parse_bound() -> int:
                bound_position = buffer.copy_position()
                bound = None
                text = None
                try:
                    text = buffer.parse_set(Set.INTEGER, consume=True)
                    bound = int(text)
                    if bound < 0:
                        raise ValueError("instance bound < 0")
                except ParseException:
                    message = "expected instance bound value"
                    errors.append(buffer.error(message))
                    return None
                except ValueError:
                    message = f"invalid instance bound value '{text}'"
                    errors.append(buffer.error(message, bound_position))
                    return None
                
                return bound

            hints = {
                "+": (1, -1),
                "*": (0, -1),
                "?": (0, 1),
            }
            for hint, bounds in hints.items():
                if buffer.match(hint, consume=True):
                    return bounds
            
            start_position = buffer.copy_position()
            if not buffer.match("<", consume=True):
                return (1, 1)
            
            lower = None
            upper = None
            colon_present = False

            buffer.skip_space()
            if buffer.read() in Set.INTEGER:
                lower = parse_bound()
                if lower is None:
                    return None
                
            buffer.skip_space()
            if buffer.match(":", consume=True):
                colon_present = True
            
            buffer.skip_space()
            if buffer.read() in Set.INTEGER:
                upper = parse_bound()
                if upper is None:
                    return None
            
            buffer.skip_space()
            if not buffer.match(">", consume=True):
                errors.append(buffer.error("expected '>'"))
                return None
            
            if (lower != None and upper != None 
                    and lower >= upper 
                    and colon_present):
                message = f"illogical instance bounds ({lower} >= {upper})"
                errors.append(buffer.error(message, start_position))
                return None
            
            elif lower and not colon_present and upper is None:
                return (lower, lower)
            elif lower and colon_present and upper is None:
                return (lower, -1)
            elif lower is None and colon_present and upper:
                return (0, upper)
            elif lower and colon_present and upper:
                return (lower, upper)
            else:
                message = "invalid instance bounds"
                errors.append(buffer.error(message, start_position))
                return None

        buffer.skip_space()
        instances = parse_instances()
        if instances is None:
            return None

        term.instances = instances
        term.predicate = predicate
        term.position = position
        term.binding = binding
        return term
    
    def bind_references(self, 
            buffer: Buffer, 
            rules: dict, 
            errors: list) -> bool:
        return True