from __future__ import annotations

from perivale import Buffer, Position


class Expression:

    def __init__(self, text: str, position: Position):
        self.text = text
        self.position = position
    
    @staticmethod
    def parse(buffer: Buffer, errors: list) -> Expression:

        buffer.skip_space()
        position = buffer.copy_position()
        if not buffer.match("{", consume=True):
            raise Exception("expected an expression")
        
        stack = 1
        text = ""
        buffer.skip_space()
        while True:

            if buffer.finished():
                message = "unexpected end-of-file before '}' in expression"
                errors.append(buffer.error(message))
                return None

            elif buffer.match("{", consume=True):
                stack += 1
                text += "{"

            elif buffer.match("}"):
                stack -= 1

                if stack:
                    text += buffer.read(consume=True)
                elif (buffer.position.line != position.line 
                        and buffer.line_indentation() != 4):
                    
                    message = "line under-indented in expression"
                    errors.append(buffer.error(message))
                    return None
                else:
                    buffer.increment()
                    break
            
            elif buffer.match("\n"):

                buffer.skip_space(include_newlines=True)
                indentation = buffer.line_indentation()
                if stack == 1 and buffer.match("}"):
                    if indentation != 4:
                        message = "line under-indented in expression"
                        errors.append(buffer.error(message))
                        return None
                    else:
                        buffer.increment()
                        break
            
                elif indentation < 8:
                    message = "line under-indented in expression"
                    errors.append(buffer.error(message))
                    return None
                
                text += "\n" + ("    " * (indentation - 8))

            else:
                text += buffer.read(consume=True)
        
        return Expression(text.strip(), position)