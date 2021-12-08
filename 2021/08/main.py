from pydantic import BaseModel


class Display(BaseModel):

    patterns: list[str]
    output: list[str]

    def validate(self):
        assert len(self.patterns) == 10
        assert len(self.output) == 4
        assert all([out in self.patterns for out in self.output])

    @classmethod
    def from_line(cls, line):
        instance = cls(
            patterns=["".join(sorted(val)) for val in line.split("|")[0].split()],
            output=["".join(sorted(val)) for val in line.split("|")[1].split()],
        )
        instance.validate()
        return instance


def count_1478(displays):
    count = 0
    for display in displays:
        count += len([out for out in display.output if len(out) in [2, 3, 4, 7]])
    return count


def read_input(filename):
    displays = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            displays.append(Display.from_line(line))
    return displays


if __name__ == "__main__":
    test_displays = read_input("test_input.txt")
    assert count_1478(test_displays) == 26
    displays = read_input("input.txt")
    print(count_1478(displays))
