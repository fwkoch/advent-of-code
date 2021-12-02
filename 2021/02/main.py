from pydantic import BaseModel

class Submarine(BaseModel):
    horiz: int = 0
    depth: int = 0

    def mult(self):
        return self.horiz * self.depth

    def instruction(self, command):
        action, value = command.split()
        value = int(value)
        if action == 'forward':
            self.horiz += value
        elif action == 'down':
            self.depth += value
        elif action == 'up':
            self.depth -= value
        else:
            raise ValueError(f'bad action: {action}')

    def parse_instructions(self, filename):
        with open(filename, 'r') as fid:
            for line in fid.readlines():
                self.instruction(line)

if __name__ == "__main__":
    sub = Submarine()
    sub.parse_instructions('./test_input.txt')
    assert sub.mult() == 150
    sub = Submarine()
    sub.parse_instructions('./input.txt')
    print(sub.mult())

