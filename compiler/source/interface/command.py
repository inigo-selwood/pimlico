import tools


class Flag:

    def __init__(self, parameters: list = [], detail: str = ''):
        self.parameters = parameters
        self.detail = detail


class Parameter:

    def __init__(self, name: str, detail: str):
        self.name = name
        self.detail = detail


class Command:

    def __init__(self, 
            name: str, 
            detail: str,
            functor: callable,
            flags: dict = {}, 
            parameters: list = []):
        
        self.name = name
        self.flags = flags
        self.parameters = parameters
        self.detail = detail
        self.functor = functor
    
    def add_flag(self, name: str, detail: str, parameters: list = []):
        assert name not in self.flags
        self.flags[name] = Flag(parameters, detail)
    
    def add_parameter(self, name: str, detail: str):
        self.parameters.append(Parameter(name, detail))

    def usage(self) -> str:

        result = f'usage: pimlico {self.name}'

        for name, flag in self.flags.items():
            hint = f'--{name}'
            for parameter in flag.parameters:
                hint += f' {parameter.upper()}'
            result += f' [{hint}]'

        for parameter in self.parameters:
            name = parameter.name.upper()
            name = name.replace('-', '_')
            name = name.replace(' ', '_')
            result += f' {name}'

        return result
    
    def help(self) -> str:

        result = f'{self.usage()}\n\n{self.detail}'
        
        if self.parameters:

            parameters = ''
            lengths = [len(parameter.name) for parameter in self.parameters]
            greatest_length = max(lengths)

            for parameter in self.parameters:
                indent = ' ' * ((greatest_length - len(parameter.name)) + 4)
                parameters += f'\n  {parameter.name}{indent}{parameter.detail}'
            
            result += f'\n\nparameters:{parameters}'

        if self.flags:

            flags = ''
            greatest_length = max([len(name) for name in self.flags])

            for name, flag in self.flags.items():
                indent = ' ' * ((greatest_length - len(name)) + 4)
                flags += f'\n  {name}{indent}{flag.detail}'
            
            result += f'\n\nflags:{flags}'

        return result
            