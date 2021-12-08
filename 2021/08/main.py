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

    def patterns_of_length(self, length):
        return [val for val in self.patterns if len(val) == length]

    def pattern_lookup(self):
        lookup = {
            1: self.patterns_of_length(2)[0],
            7: self.patterns_of_length(3)[0],
            4: self.patterns_of_length(4)[0],
            8: self.patterns_of_length(7)[0],
        }
        for pattern in self.patterns_of_length(6):
            if not all([val in pattern for val in lookup[1]]):
                lookup[6] = pattern
        for pattern in self.patterns_of_length(5):
            if all([val in lookup[6] for val in pattern]):
                lookup[5] = pattern
        for pattern in self.patterns_of_length(5):
            if pattern == lookup[5]:
                continue
            if sum([val in pattern for val in lookup[5]]) == 3:
                lookup[2] = pattern
            else:
                lookup[3] = pattern
        for pattern in self.patterns_of_length(6):
            if pattern == lookup[6]:
                continue
            if all([val in pattern for val in lookup[5]]):
                lookup[9] = pattern
            else:
                lookup[0] = pattern
        return {val: key for key, val in lookup.items()}

    def value(self):
        lookup = self.pattern_lookup()
        return (
            lookup[self.output[0]] * 1000
            + lookup[self.output[1]] * 100
            + lookup[self.output[2]] * 10
            + lookup[self.output[3]]
        )


def count_1478(displays):
    count = 0
    for display in displays:
        count += len([out for out in display.output if len(out) in [2, 3, 4, 7]])
    return count


def count_values(displays):
    return sum([display.value() for display in displays])


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
    assert count_values(test_displays) == 61229
    print(count_values(displays))
