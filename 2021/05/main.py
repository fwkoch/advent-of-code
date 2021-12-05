from pydantic import BaseModel


class VentLine(BaseModel):

    start: tuple[int, int]
    end: tuple[int, int]

    @classmethod
    def from_input(cls, line):
        return cls(
            start=tuple(int(val) for val in line.split()[0].split(",")),
            end=tuple(int(val) for val in line.split()[2].split(",")),
        )

    @property
    def horizontal(self):
        return self.start[1] == self.end[1]

    @property
    def vertical(self):
        return self.start[0] == self.end[0]

    def points(self):
        if self.horizontal:
            a = min(self.start[0], self.end[0])
            b = max(self.start[0], self.end[0])
            return [(i, self.start[1]) for i in range(a, b + 1)]
        if self.vertical:
            a = min(self.start[1], self.end[1])
            b = max(self.start[1], self.end[1])
            return [(self.start[0], i) for i in range(a, b + 1)]
        return []

    def overlap(self, other):
        return set(self.points()).intersection(other.points())


def load_vents(filename):
    lines = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            lines.append(VentLine.from_input(line))
    return lines


def count_overlaps(lines):
    overlaps = set()
    for i in range(len(lines)):
        for other in lines[i + 1 :]:
            overlaps = overlaps.union(lines[i].overlap(other))
    return len(overlaps)


if __name__ == "__main__":
    test_vents = load_vents("./test_input.txt")
    assert count_overlaps(test_vents) == 5
    vents = load_vents("./input.txt")
    print(count_overlaps(vents))
