from pydantic import BaseModel


class Submarine(BaseModel):
    horiz: int = 0
    depth: int = 0

    def mult(self):
        return self.horiz * self.depth

    def instruction(self, command):
        action, value = command.split()
        value = int(value)
        if action == "forward":
            self.forward(value)
        elif action == "down":
            self.down(value)
        elif action == "up":
            self.up(value)
        else:
            raise ValueError(f"bad action: {action}")

    def parse_instructions(self, filename):
        with open(filename, "r") as fid:
            for line in fid.readlines():
                self.instruction(line)

    def forward(self, value):
        self.horiz += value

    def down(self, value):
        self.depth += value

    def up(self, value):
        self.depth -= value


class ActualSubmarine(Submarine):
    aim: int = 0

    def forward(self, value):
        self.horiz += value
        self.depth += self.aim * value

    def down(self, value):
        self.aim += value

    def up(self, value):
        self.aim -= value


if __name__ == "__main__":
    sub = Submarine()
    sub.parse_instructions("./test_input.txt")
    assert sub.mult() == 150
    sub = Submarine()
    sub.parse_instructions("./input.txt")
    print(sub.mult())
    sub = ActualSubmarine()
    sub.parse_instructions("./test_input.txt")
    assert sub.mult() == 900
    sub = ActualSubmarine()
    sub.parse_instructions("./input.txt")
    print(sub.mult())
