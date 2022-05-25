class Position:

    def __init__(self):
        self.index = 0
        self.line = 1
        self.column = 1
    
    def __str__(self):
        return f'[{self.line}:{self.column}]'